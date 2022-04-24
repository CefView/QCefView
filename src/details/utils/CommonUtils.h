#pragma once
#include <QMetaType>

#define REGISTER_METATYPE(TYPE)                                                                                        \
  static struct TYPE##_MetaTypeRegister                                                                                \
  {                                                                                                                    \
    TYPE##_MetaTypeRegister() { qRegisterMetaType<TYPE>(); }                                                           \
  } TYPE##_MetaTypeRegister;
