#include "ValueConvertor.h"

#pragma region qt_headers
#include <QDebug>
#pragma endregion

void
ValueConvertor::CefValueToQVariant(QVariant* qVariant, CefValue* cValue)
{
  if (!qVariant || !cValue) {
    qWarning() << "Invalid arguments";
    return;
  }

  auto type = cValue->GetType();
  switch (type) {
    case CefValueType::VTYPE_INVALID:
    case CefValueType::VTYPE_NULL: {
      qVariant->setValue(QVariant());
    } break;
    case CefValueType::VTYPE_BOOL: {
      qVariant->setValue<bool>(cValue->GetBool());
    } break;
    case CefValueType::VTYPE_INT: {
      qVariant->setValue<int>(cValue->GetInt());
    } break;
    case CefValueType::VTYPE_DOUBLE: {
      qVariant->setValue<double>(cValue->GetDouble());
    } break;
    case CefValueType::VTYPE_STRING: {
#if defined(CEF_STRING_TYPE_UTF16)
      QString qStr = QString::fromUtf16((char16_t*)cValue->GetString().c_str());
#elif defined(CEF_STRING_TYPE_UTF8)
      QString qStr = QString::fromUtf8(cValue->GetString().c_str());
#elif defined(CEF_STRING_TYPE_WIDE)
      QString qStr = QString::fromWCharArray(cValue->GetString().c_str());
#endif
      qVariant->setValue(qStr);
    } break;
    case CefValueType::VTYPE_BINARY: {
      auto cData = cValue->GetBinary();
      auto cLen = cData->GetSize();
      QByteArray qData((int)cLen, 0);
      cValue->GetBinary()->GetData(qData.data(), qData.size(), 0);
      qVariant->setValue(qData);
    } break;
    case CefValueType::VTYPE_DICTIONARY: {
      auto cDict = cValue->GetDictionary();
      CefDictionaryValue::KeyList cKeys;
      if (!cDict->GetKeys(cKeys)) {
        qWarning() << "Failed to get cef dictionary keys";
      }
      QVariantMap qDict;
      for (auto& key : cKeys) {
        auto cVal = cDict->GetValue(key);
        QVariant qVal;
        CefValueToQVariant(&qVal, cVal.get());
#if defined(CEF_STRING_TYPE_UTF16)
        QString qKey = QString::fromUtf16((char16_t*)key.c_str());
#elif defined(CEF_STRING_TYPE_UTF8)
        QString qKey = QString::fromUtf8(key.c_str());
#elif defined(CEF_STRING_TYPE_WIDE)
        QString qKey = QString::fromWCharArray(key.c_str());
#endif
        qDict.insert(qKey, qVal);
      }
      qVariant->setValue(qDict);
    } break;
    case CefValueType::VTYPE_LIST: {
      auto cList = cValue->GetList();
      auto cCount = cList->GetSize();
      QVariantList qList;
      for (int i = 0; i < cCount; i++) {
        auto cVal = cList->GetValue(i);
        QVariant qVal;
        CefValueToQVariant(&qVal, cVal.get());
        qList.push_back(qVal);
      }
      qVariant->setValue(qList);
    } break;
    default:
      qWarning() << "Unsupported CefValueType conversion: " << type;
  }
}

void
ValueConvertor::QVariantToCefValue(CefValue* cValue, const QVariant* qVariant)
{
  if (!qVariant || !cValue) {
    qWarning() << "Invalid arguments";
    return;
  }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  auto type = qVariant->type();
#define TypeClass QVariant
#define UndefinedType Invalid
#define StringType String
#define ByteArrayType ByteArray
#define VariantMapType Map
#define VariantListType List
#else
  auto type = qVariant->typeId();
#define TypeClass QMetaType
#define UndefinedType UnknownType
#define StringType QString
#define ByteArrayType QByteArray
#define VariantMapType QVariantMap
#define VariantListType QVariantList
#endif

  switch (type) {
    case TypeClass::UndefinedType: {
      cValue->SetNull();
    } break;
    case TypeClass::Bool: {
      cValue->SetBool(qVariant->toBool());
    } break;
    case TypeClass::Char:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case TypeClass::UChar:
    case TypeClass::Short:
    case TypeClass::UShort:
#endif
    case TypeClass::Int: {
      cValue->SetInt(qVariant->toInt());
    } break;
    case TypeClass::UInt:
    case TypeClass::LongLong:
    case TypeClass::ULongLong:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    case TypeClass::Float:
#endif
    case TypeClass::Double: {
      cValue->SetDouble(qVariant->toDouble());
    } break;
    case TypeClass::StringType: {
      CefString cStr = CefString(qVariant->toString().toStdString());
      cValue->SetString(cStr);
    } break;
    case TypeClass::ByteArrayType: {
      auto qData = qVariant->toByteArray();
      auto cData = CefBinaryValue::Create(qData.constData(), qData.size());
      cValue->SetBinary(cData);
    } break;
    case TypeClass::VariantMapType: {
      auto qDict = qVariant->toMap();
      auto keys = qDict.keys();
      auto cDict = CefDictionaryValue::Create();
      for (auto& key : keys) {
        auto qVal = qDict.take(key);
        auto cVal = CefValue::Create();
        QVariantToCefValue(cVal.get(), &qVal);
        CefString cKey = CefString(key.toStdString());
        cDict->SetValue(cKey, cVal);
      }
      cValue->SetDictionary(cDict);
    } break;
    case TypeClass::VariantListType: {
      auto qList = qVariant->toList();
      auto qCount = qList.size();
      auto cList = CefListValue::Create();
      for (int i = 0; i < qCount; i++) {
        auto qVal = qList.at(i);
        auto cVal = CefValue::Create();
        QVariantToCefValue(cVal.get(), &qVal);
        cList->SetValue(i, cVal);
      }
      cValue->SetList(cList);
    } break;
    default:
      qWarning() << "Unsupported QVariantType conversion: " << type;
      break;
  }
}

QCefFrameId
ValueConvertor::FrameIdC2Q(const CefFrameId& id)
{
#if CEF_VERSION_MAJOR < 122
  return id;
#else
  return QCefFrameId::fromUtf8(id.ToString().c_str(), static_cast<int>(id.ToString().length()));
#endif
}

CefFrameId
ValueConvertor::FrameIdQ2C(const QCefFrameId& id)
{
#if CEF_VERSION_MAJOR < 122
  return id;
#else
  return CefFrameId(id.toStdString());
#endif
}
