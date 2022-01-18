#include "QCefSetting.h"

#include "details/QCefSettingPrivate.h"

#pragma region qt_headers
#pragma endregion qt_headers

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
