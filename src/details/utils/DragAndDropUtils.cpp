#include "DragAndDropUtils.h"

#include <QList>
#include <QMimeData>
#include <QPixmap>
#include <QPoint>
#include <QUrl>

class BytesWriteHandler : public CefWriteHandler
{
public:
  explicit BytesWriteHandler(size_t grow)
    : grow_(grow)
    , datasize_(grow)
  {
    DCHECK_GT(grow, 0U);
    data_ = malloc(grow);
    DCHECK(data_ != nullptr);
  }

  ~BytesWriteHandler() override
  {
    if (data_) {
      free(data_);
    }
  }

  size_t Write(const void* ptr, size_t size, size_t n) override
  {
    base::AutoLock lock_scope(lock_);
    size_t rv;
    if (offset_ + static_cast<int64_t>(size * n) >= datasize_ && Grow(size * n) == 0) {
      rv = 0;
    } else {
      memcpy(reinterpret_cast<char*>(data_) + offset_, ptr, size * n);
      offset_ += size * n;
      rv = n;
    }

    return rv;
  }

  int Seek(int64_t offset, int whence) override
  {
    int rv = -1L;
    base::AutoLock lock_scope(lock_);
    switch (whence) {
      case SEEK_CUR:
        if (offset_ + offset > datasize_ || offset_ + offset < 0) {
          break;
        }
        offset_ += offset;
        rv = 0;
        break;
      case SEEK_END: {
        int64_t offset_abs = std::abs(offset);
        if (offset_abs > datasize_) {
          break;
        }
        offset_ = datasize_ - offset_abs;
        rv = 0;
        break;
      }
      case SEEK_SET:
        if (offset > datasize_ || offset < 0) {
          break;
        }
        offset_ = offset;
        rv = 0;
        break;
    }

    return rv;
  }

  int64_t Tell() override
  {
    base::AutoLock lock_scope(lock_);
    return offset_;
  }

  int Flush() override { return 0; }

  bool MayBlock() override { return false; }

  void* GetData() { return data_; }
  int64_t GetDataSize() { return offset_; }

private:
  size_t Grow(size_t size)
  {
    lock_.AssertAcquired();
    size_t rv;
    size_t s = (size > grow_ ? size : grow_);
    void* tmp = realloc(data_, datasize_ + s);
    DCHECK(tmp != nullptr);
    if (tmp) {
      data_ = tmp;
      datasize_ += s;
      rv = datasize_;
    } else {
      rv = 0;
    }

    return rv;
  }

  size_t grow_;
  void* data_;
  int64_t datasize_;
  int64_t offset_ = 0;

  base::Lock lock_;

  IMPLEMENT_REFCOUNTING(BytesWriteHandler);
  DISALLOW_COPY_AND_ASSIGN(BytesWriteHandler);
};

Qt::DropActions
MapCefDragOperationToQDropAction(CefRenderHandler::DragOperationsMask operation)
{
  Qt::DropActions dropActions = Qt::IgnoreAction;

  if (operation & DRAG_OPERATION_COPY) {
    dropActions |= Qt::CopyAction;
  }

  if (operation & DRAG_OPERATION_MOVE) {
    dropActions |= Qt::MoveAction;
  }

  if (operation & DRAG_OPERATION_LINK) {
    dropActions |= Qt::LinkAction;
  }

  return dropActions;
}

QMimeData*
CreateQMimeDataFromCefDragData(CefDragData& dragData)
{
  QList<QUrl> urls;
  QMimeData* mimeData = new QMimeData();

  CefString fragmentText = dragData.GetFragmentText();
  if (!fragmentText.empty()) {
    mimeData->setText(QString::fromStdString(fragmentText.ToString()));
  }

  CefString fragmentHtml = dragData.GetFragmentHtml();
  if (!fragmentHtml.empty()) {
    mimeData->setHtml(QString::fromStdString(fragmentHtml.ToString()));
  }

  // CefString fragmentBaseUrl = dragData.GetFragmentBaseURL();
  // if (!fragmentBaseUrl.empty()) {
  //   urls.append(QString::fromUtf8(fragmentBaseUrl.ToString()));
  // }

  CefString link = dragData.GetLinkURL();
  if (!link.empty()) {
    urls.append(QString::fromStdString(link.ToString()));
  }

  size_t bufferSize = dragData.GetFileContents(nullptr);
  if (bufferSize) {
    CefString names = dragData.GetFileName();
    CefRefPtr<BytesWriteHandler> handler = new BytesWriteHandler(bufferSize);
    CefRefPtr<CefStreamWriter> writer = CefStreamWriter::CreateForHandler(handler.get());
    dragData.GetFileContents(writer);
    QByteArray fileContent(static_cast<char*>(handler->GetData()), handler->GetDataSize());
    mimeData->setData("application/octet-stream", fileContent);
  }

  mimeData->setUrls(urls);

  return mimeData;
}

CefRenderHandler::DragOperationsMask
MapQDropActionToCefDragOperation(Qt::DropActions actions)
{
  int dragOps = DRAG_OPERATION_NONE;

  if (actions & Qt::CopyAction) {
    dragOps |= DRAG_OPERATION_COPY;
  }

  if (actions & Qt::MoveAction) {
    dragOps |= DRAG_OPERATION_MOVE;
  }

  if (actions & Qt::LinkAction) {
    dragOps |= DRAG_OPERATION_LINK;
  }

  return static_cast<CefRenderHandler::DragOperationsMask>(dragOps);
}

CefRefPtr<CefDragData>
CreateCefDragDataFromQMimeData(const QMimeData& mime)
{
  CefRefPtr<CefDragData> dragData = CefDragData::Create();

  if (mime.hasUrls() && !mime.urls().isEmpty()) {
    dragData->SetLinkURL(mime.urls().takeFirst().toString().toStdString());
  }

  if (mime.hasText()) {
    dragData->SetFragmentText(mime.text().toStdString());
  }

  if (mime.hasHtml()) {
    dragData->SetFragmentHtml(mime.html().toStdString());
  }

  return dragData;
}
