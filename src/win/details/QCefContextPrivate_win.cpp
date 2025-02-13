#include "../../details/QCefContextPrivate.h"

#include <Shlwapi.h>

#include <QDebug>
#include <QDir>

#include <CefViewCoreProtocol.h>

#include "../../details/QCefConfigPrivate.h"

bool
QCefContextPrivate::initializeCef(const QCefConfig* config)
{
  std::vector<wchar_t> modPath(MAX_PATH * 4);
  ::GetModuleFileNameW(nullptr, modPath.data(), static_cast<DWORD>(modPath.size()));
  ::PathRemoveFileSpecW(modPath.data());
  ::PathCombineW(modPath.data(), modPath.data(), L"CefView");
  ::SetDllDirectoryW(modPath.data());

#if CEF_VERSION_MAJOR < 112
  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();
#endif

  // Build CefSettings
  CefSettings cef_settings;
  QCefConfigPrivate::CopyToCefSettings(config, &cef_settings);

  if (CefString(&cef_settings.browser_subprocess_path).empty()) {
    QString strExePath = QDir(QString::fromWCharArray(modPath.data())).filePath(kCefViewRenderProcessName);
    CefString(&cef_settings.browser_subprocess_path) = QDir::toNativeSeparators(strExePath).toStdString();
  }

#if CEF_VERSION_MAJOR >= 125 && CEF_VERSION_MAJOR <= 127
  //  https://github.com/chromiumembedded/cef/issues/3685
  cef_settings.chrome_runtime = true;
#endif

  // fixed values
#if CEF_VERSION_MAJOR < 128
  cef_settings.pack_loading_disabled = false;
#endif

#if defined(CEF_USE_QT_EVENT_LOOP)
  cef_settings.multi_threaded_message_loop = false;
  cef_settings.external_message_pump = true;
#else
  cef_settings.multi_threaded_message_loop = true;
  cef_settings.external_message_pump = false;
#endif

#if !defined(CEF_USE_SANDBOX)
  cef_settings.no_sandbox = true;
#endif

  // create job object
  DWORD dwProcessId = ::GetProcessId(::GetCurrentProcess());
  windowsJobName_ = QString("CefView-Job-{f0a3c1e3-ff89-4581-8a45-f0bfd74c4bb0}-%1").arg(dwProcessId);
  windowsJobHandle_ = ::CreateJobObjectA(nullptr, windowsJobName_.toStdString().c_str());
  if (nullptr == windowsJobHandle_) {
    qWarning() << "Failed to create windows job object:" << ::GetLastError();
  } else {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    ::memset(&info, 0, sizeof(info));
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;
    if (!::SetInformationJobObject(windowsJobHandle_, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
      qWarning() << "Failed to set information for windows job object:" << GetLastError();
    }
    if (!::AssignProcessToJobObject(windowsJobHandle_, ::GetCurrentProcess())) {
      qWarning() << "Failed to assign current process to windows job object:" << ::GetLastError();
    }
  }

  // Initialize CEF
  auto cmdArgs = QCefConfigPrivate::GetCommandLineArgs(config);
  cmdArgs[kCefViewWindowsJobNameKey] = windowsJobName_.toStdString();
  auto appDelegate = std::make_shared<CCefAppDelegate>(this, cmdArgs);
  auto builtinSchemeName = config ? config->builtinSchemeName().toStdString() : std::string();
  auto bridgeObjectName = config ? config->bridgeObjectName().toStdString() : std::string();
  auto app = new CefViewBrowserApp(builtinSchemeName, bridgeObjectName, appDelegate);

  void* sandboxInfo = nullptr;
#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  static CefScopedSandboxInfo scopedSandbox;
  sandboxInfo = scopedSandbox.sandbox_info();
#endif

  CefMainArgs main_args(::GetModuleHandle(nullptr));
  if (!CefInitialize(main_args, cef_settings, app, sandboxInfo)) {
    assert(0);
    return false;
  }

  pApp_ = app;
  pAppDelegate_ = appDelegate;

  return true;
}

void
QCefContextPrivate::uninitializeCef()
{
  if (!pApp_)
    return;

  pAppDelegate_ = nullptr;
  pApp_ = nullptr;

  // shutdown the cef
  CefShutdown();

  // clean job object
  if (windowsJobHandle_) {
    ::CloseHandle(windowsJobHandle_);
    windowsJobHandle_ = nullptr;
  }
}
