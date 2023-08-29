#pragma once
#include <QDebug>
#include <QMetaType>
#include <QRegion>
#include <QWindow>

#define REGISTER_METATYPE(TYPE)                                                                                        \
  static struct TYPE##_MetaTypeRegister                                                                                \
  {                                                                                                                    \
    TYPE##_MetaTypeRegister()                                                                                          \
    {                                                                                                                  \
      qRegisterMetaType<TYPE>();                                                                                       \
    }                                                                                                                  \
  } TYPE##_MetaTypeRegister;

class FunctionLogger
{
public:
  FunctionLogger(const QString& fn)
    : functionName_(fn)
  {
    qDebug() << "+++" << functionName_;
  }

  ~FunctionLogger() { qDebug() << "---" << functionName_; }

  QString functionName_;
};

#if defined(QT_DEBUG)
#define FLog() FunctionLogger __fl__(__FUNCTION__);
#else
#define FLog()
#endif
