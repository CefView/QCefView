#include "CCefClientDelegate.h"

#include <QDebug>
#include <QScreen>

#include "QCefSettingPrivate.h"
#include "QCefViewPrivate.h"
#include "utils/ValueConvertor.h"

CCefClientDelegate::CCefClientDelegate(QCefViewPrivate* p)
  : pCefViewPrivate_(p)
{
}

CCefClientDelegate::~CCefClientDelegate()
{
  qDebug() << "CCefClientDelegate is being destructed";
}

void
CCefClientDelegate::processUrlRequest(CefRefPtr<CefBrowser>& browser, const CefFrameId& frameId, const std::string& url)
{
  if (!IsValidBrowser(browser))
    return;

  auto browserId = browser->GetIdentifier();
  auto u = QString::fromStdString(url);
  auto source = pCefViewPrivate_->q_ptr;

  emit source->cefUrlRequest(browserId, ValueConvertor::FrameIdC2Q(frameId), u);
}

void
CCefClientDelegate::processQueryRequest(CefRefPtr<CefBrowser>& browser,
                                        const CefFrameId& frameId,
                                        const std::string& request,
                                        const int64_t query_id)
{
  if (!IsValidBrowser(browser))
    return;

  auto browserId = browser->GetIdentifier();
  auto req = QString::fromStdString(request);
  auto source = pCefViewPrivate_->q_ptr;
  auto query = pCefViewPrivate_->createQuery(req, query_id);

  emit source->cefQueryRequest(browserId, ValueConvertor::FrameIdC2Q(frameId), query);
}

void
CCefClientDelegate::focusedEditableNodeChanged(CefRefPtr<CefBrowser>& browser,
                                               const CefFrameId& frameId,
                                               bool focusOnEditableNode)
{
  if (!IsValidBrowser(browser))
    return;

  QMetaObject::invokeMethod(pCefViewPrivate_,                //
                            "onCefInputStateChanged",        //
                            Q_ARG(bool, focusOnEditableNode) //
  );
}

void
CCefClientDelegate::invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                       const CefFrameId& frameId,
                                       const std::string& method,
                                       const CefRefPtr<CefListValue>& arguments)
{
  if (!IsValidBrowser(browser))
    return;

  auto m = QString::fromStdString(method);
  QVariantList args;
  for (int i = 0; i < arguments->GetSize(); i++) {
    QVariant qV;
    auto cV = arguments->GetValue(i);
    ValueConvertor::CefValueToQVariant(&qV, cV.get());
    args.push_back(qV);
  }

  auto browserId = browser->GetIdentifier();
  emit pCefViewPrivate_->q_ptr->invokeMethod(browserId, ValueConvertor::FrameIdC2Q(frameId), m, args);
}

void
CCefClientDelegate::reportJSResult(CefRefPtr<CefBrowser>& browser,
                                   const CefFrameId& frameId,
                                   const std::string& context,
                                   const CefRefPtr<CefValue>& result)
{
  if (!IsValidBrowser(browser))
    return;

  auto browserId = browser->GetIdentifier();
  QVariant qV;
  ValueConvertor::CefValueToQVariant(&qV, result.get());
  auto c = QString::fromStdString(context);
  emit pCefViewPrivate_->q_ptr->reportJavascriptResult(browserId, ValueConvertor::FrameIdC2Q(frameId), c, qV);
}
