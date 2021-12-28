#include <QCef.h>

#include "details/CCefManager.h"

namespace QCef {

static CCefManagerPtr g_cefManager = nullptr;

bool
Initialize(const QCefSetting& settings, int argc, char* argv[])
{
  // create manager instance
  if (!g_cefManager) {
    g_cefManager = std::make_shared<CCefManager>(argc, argv, settings);
  }

  if (g_cefManager) {
    g_cefManager->setInstnace(g_cefManager);
    return true;
  }

  return false;
}

void
Uninitialize()
{
  if (g_cefManager) {
    g_cefManager.reset();
  }
}
} // namespace QCef
