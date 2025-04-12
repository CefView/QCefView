#include <QCefSetting.h>

#pragma region cef_headers
#include <include/cef_version.h>
#pragma endregion 

#include "details/QCefSettingPrivate.h"
#include "details/utils/CommonUtils.h"

REGISTER_METATYPE(QCefSetting);

QCefSetting::QCefSetting()
  : d_ptr(new QCefSettingPrivate)
{
}

QCefSetting::QCefSetting(const QCefSetting& other)
  : d_ptr(new QCefSettingPrivate)
{
  *d_ptr = *(other.d_ptr);
}

QCefSetting&
QCefSetting::operator=(const QCefSetting& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

QCefSetting::~QCefSetting() {}

void
QCefSetting::setWindowInitialSize(const QSize& size)
{
  Q_D(QCefSetting);
  d->windowInitialSize_ = size;
}

const QSize
QCefSetting::windowInitialSize() const
{
  Q_D(const QCefSetting);
  return d->windowInitialSize_;
}

void
QCefSetting::setStandardFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->standardFontFamily_ = value.toStdString();
}

const QString
QCefSetting::standardFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->standardFontFamily_);
}

void
QCefSetting::setFixedFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->fixedFontFamily_ = value.toStdString();
}

const QString
QCefSetting::fixedFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->fixedFontFamily_);
}

void
QCefSetting::setSerifFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->serifFontFamily_ = value.toStdString();
}

const QString
QCefSetting::serifFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->serifFontFamily_);
}

void
QCefSetting::setSansSerifFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->sansSerifFontFamily_ = value.toStdString();
}

const QString
QCefSetting::sansSerifFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->sansSerifFontFamily_);
}

void
QCefSetting::setCursiveFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->cursiveFontFamily_ = value.toStdString();
}

const QString
QCefSetting::cursiveFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->cursiveFontFamily_);
}

void
QCefSetting::setFantasyFontFamily(const QString& value)
{
  Q_D(QCefSetting);
  d->fantasyFontFamily_ = value.toStdString();
}

const QString
QCefSetting::fantasyFontFamily() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->fantasyFontFamily_);
}

void
QCefSetting::setDefaultEncoding(const QString& value)
{
  Q_D(QCefSetting);
  d->defaultEncoding_ = value.toStdString();
}

const QString
QCefSetting::defaultEncoding() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->defaultEncoding_);
}

#if CEF_VERSION_MAJOR < 118
void
QCefSetting::setAcceptLanguageList(const QString& value)
{
  Q_D(QCefSetting);
  d->acceptLanguageList_ = value.toStdString();
}

const QString
QCefSetting::acceptLanguageList() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->acceptLanguageList_);
}
#endif

void
QCefSetting::setWindowlessFrameRate(const int value)
{
  Q_D(QCefSetting);
  d->windowlessFrameRate_ = value;
}

const QVariant
QCefSetting::windowlessFrameRate() const
{
  Q_D(const QCefSetting);
  return d->windowlessFrameRate_;
}

void
QCefSetting::setDefaultFontSize(const int value)
{
  Q_D(QCefSetting);
  d->defaultFontSize_ = value;
}

const QVariant
QCefSetting::defaultFontSize() const
{
  Q_D(const QCefSetting);
  return d->defaultFontSize_;
}

void
QCefSetting::setDefaultFixedFontSize(const int value)
{
  Q_D(QCefSetting);
  d->defaultFixedFontSize_ = value;
}

const QVariant
QCefSetting::defaultFixedFontSize() const
{
  Q_D(const QCefSetting);
  return d->defaultFixedFontSize_;
}

void
QCefSetting::setMinimumFontSize(const int value)
{
  Q_D(QCefSetting);
  d->minimumFontSize_ = value;
}

const QVariant
QCefSetting::minimumFontSize() const
{
  Q_D(const QCefSetting);
  return d->minimumFontSize_;
}

void
QCefSetting::setMinimumLogicalFontSize(const int value)
{
  Q_D(QCefSetting);
  d->minimumLogicalFontSize_ = value;
}

const QVariant
QCefSetting::minimumLogicalFontSize() const
{
  Q_D(const QCefSetting);
  return d->minimumLogicalFontSize_;
}

void
QCefSetting::setRemoteFonts(const bool value)
{
  Q_D(QCefSetting);
  d->remoteFonts_ = value;
}

const QVariant
QCefSetting::remoteFonts() const
{
  Q_D(const QCefSetting);
  return d->remoteFonts_;
}

void
QCefSetting::setJavascript(const bool value)
{
  Q_D(QCefSetting);
  d->javascript_ = value;
}

const QVariant
QCefSetting::javascript() const
{
  Q_D(const QCefSetting);
  return d->javascript_;
}

void
QCefSetting::setJavascriptCloseWindows(const bool value)
{
  Q_D(QCefSetting);
  d->javascriptCloseWindows_ = value;
}

const QVariant
QCefSetting::javascriptCloseWindows() const
{
  Q_D(const QCefSetting);
  return d->javascriptCloseWindows_;
}

void
QCefSetting::setJavascriptAccessClipboard(const bool value)
{
  Q_D(QCefSetting);
  d->javascriptAccessClipboard_ = value;
}

const QVariant
QCefSetting::javascriptAccessClipboard() const
{
  Q_D(const QCefSetting);
  return d->javascriptAccessClipboard_;
}

void
QCefSetting::setJavascriptDomPaste(const bool value)
{
  Q_D(QCefSetting);
  d->javascriptDomPaste_ = value;
}

const QVariant
QCefSetting::javascriptDomPaste() const
{
  Q_D(const QCefSetting);
  return d->javascriptDomPaste_;
}

#if CEF_VERSION_MAJOR < 100
void
QCefSetting::setPlugins(const bool value)
{
  Q_D(QCefSetting);
  d->plugins_ = value;
}

const QVariant
QCefSetting::plugins() const
{
  Q_D(const QCefSetting);
  return d->plugins_;
}
#endif

void
QCefSetting::setImageLoading(const bool value)
{
  Q_D(QCefSetting);
  d->imageLoading_ = value;
}

const QVariant
QCefSetting::imageLoading() const
{
  Q_D(const QCefSetting);
  return d->imageLoading_;
}

void
QCefSetting::setImageShrinkStandaloneToFit(const bool value)
{
  Q_D(QCefSetting);
  d->imageShrinkStandaloneToFit_ = value;
}

const QVariant
QCefSetting::imageShrinkStandaloneToFit() const
{
  Q_D(const QCefSetting);
  return d->imageShrinkStandaloneToFit_;
}

void
QCefSetting::setTextAreaResize(const bool value)
{
  Q_D(QCefSetting);
  d->textAreaResize_ = value;
}

const QVariant
QCefSetting::textAreaResize() const
{
  Q_D(const QCefSetting);
  return d->textAreaResize_;
}

void
QCefSetting::setTabToLinks(const bool value)
{
  Q_D(QCefSetting);
  d->tabToLinks_ = value;
}

const QVariant
QCefSetting::tabToLinks() const
{
  Q_D(const QCefSetting);
  return d->tabToLinks_;
}

void
QCefSetting::setLocalStorage(const bool value)
{
  Q_D(QCefSetting);
  d->localStorage_ = value;
}

const QVariant
QCefSetting::localStorage() const
{
  Q_D(const QCefSetting);
  return d->localStorage_;
}

void
QCefSetting::setDatabases(const bool value)
{
  Q_D(QCefSetting);
  d->databases_ = value;
}

const QVariant
QCefSetting::databases() const
{
  Q_D(const QCefSetting);
  return d->databases_;
}

void
QCefSetting::setWebGL(const bool value)
{
  Q_D(QCefSetting);
  d->webgl_ = value;
}

const QVariant
QCefSetting::webGL() const
{
  Q_D(const QCefSetting);
  return d->webgl_;
}

void
QCefSetting::setBackgroundColor(const QColor& value)
{
  Q_D(QCefSetting);
  d->backgroundColor_ = value;
}

const QVariant
QCefSetting::backgroundColor() const
{
  Q_D(const QCefSetting);
  return d->backgroundColor_;
}

#if CEF_VERSION_MAJOR >= 125
void
QCefSetting::setHardwareAcceleration(const bool value)
{
  Q_D(QCefSetting);
  d->hardwareAcceleration_ = value;
}

const bool
QCefSetting::hardwareAcceleration() const
{
  Q_D(const QCefSetting);
  return d->hardwareAcceleration_;
}
#endif
