#include "CCefClientDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

void
CCefClientDelegate::onResourceLoadComplete(CefRefPtr<CefBrowser> browser,
                                           CefRefPtr<CefFrame> frame,
                                           CefRefPtr<CefRequest> request,
                                           CefRefPtr<CefResponse> response,
                                           CefResourceRequestHandler::URLRequestStatus status,
                                           int64 received_content_length)
{
    if (pCefViewPrivate_)
    {
      return pCefViewPrivate_->onResourceLoadComplete(browser, frame, request, response, status, received_content_length);
    }
}

CefRefPtr<CefResponseFilter>
CCefClientDelegate::onGetResourceResponseFilter(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefRefPtr<CefRequest> request,
                                              CefRefPtr<CefResponse> response)
{
  if (pCefViewPrivate_)
  {
    return pCefViewPrivate_->onGetResourceResponseFilter(browser, frame, request, response);
  }

  return nullptr;
}
