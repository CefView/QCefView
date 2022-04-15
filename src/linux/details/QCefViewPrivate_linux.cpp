#include "../../details/QCefViewPrivate.h"

bool
QCefViewPrivate::nativeEventFilter(const QByteArray& eventType,
                                   void* m,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                   qintptr* result
#else
                                   long* result
#endif
)
{
#if defined(CEF_USE_OSR)
  return false;
#else
  return false;
#endif
}
