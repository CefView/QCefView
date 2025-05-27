#pragma once

#pragma region qt_headers
#include <QDebug>
#include <QMetaType>
#include <QRegion>
#include <QWindow>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

/// <summary>
/// Registers a Qt metatype for the specified type.
/// This macro defines a static struct whose constructor registers the given type
/// with Qt's meta-object system using qRegisterMetaType<TYPE>().
/// </summary>
/// <remarks>
/// Usage:
/// REGISTER_METATYPE(MyCustomType)
/// </remarks>
#define REGISTER_METATYPE(TYPE)                                                                                        \
  static struct TYPE##_MetaTypeRegister                                                                                \
  {                                                                                                                    \
    TYPE##_MetaTypeRegister()                                                                                          \
    {                                                                                                                  \
      qRegisterMetaType<TYPE>();                                                                                       \
    }                                                                                                                  \
  } TYPE##_MetaTypeRegister;

/// <summary>
/// Utility class for logging function entry and exit.
/// Prints debug messages upon construction and destruction,
/// indicating when a function is entered and exited.
/// </summary>
class FunctionLogger
{
public:
  /// <summary>
  /// Constructs a FunctionLogger for the given function name.
  /// </summary>
  /// <param name="fn">The name of the function to log.</param>
  FunctionLogger(const QString& fn)
    : functionName_(fn)
  {
    qDebug() << "+++" << functionName_;
  }

  /// <summary>
  /// Destructor logs function exit.
  /// </summary>
  ~FunctionLogger() { qDebug() << "---" << functionName_; }

  /// <summary>
  /// The name of the function being logged.
  /// </summary>
  QString functionName_;
};

#if defined(QT_DEBUG)
/// <summary>
/// Instantiates a FunctionLogger for the current function.
/// In debug builds, this macro creates a FunctionLogger named __fl__ using
/// the current function's name. In release builds, it does nothing.
/// </summary>
#define FLog() FunctionLogger __fl__(__FUNCTION__);
#else
#define FLog()
#endif
