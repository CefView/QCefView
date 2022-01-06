#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QCefView.h>

/// <summary>
/// Represents the customized QCefView
/// </summary>
class CustomCefView : public QCefView
{
  Q_OBJECT

public:
  CustomCefView(const QString url, QWidget* parent = 0);

  ~CustomCefView();

  /// <summary>
  /// Changes the background color of web area
  /// </summary>
  void changeColor();

protected:
  //#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  //  virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
  //#else
  //  virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
  //#endif

  /// <summary>
  /// Gets called on draggable region on web area changed
  /// </summary>
  /// <param name="draggableRegion"></param>
  /// <param name="nonDraggableRegion"></param>
  virtual void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion) override;

  /// <summary>
  /// Gets called on url request from web
  /// </summary>
  /// <param name="url">The url</param>
  virtual void onQCefUrlRequest(const QString& url) override;

  /// <summary>
  /// Gets called on query request from web
  /// </summary>
  /// <param name="query">The query object</param>
  virtual void onQCefQueryRequest(const QCefQuery& query) override;

  /// <summary>
  /// Gets called on invocation request from web
  /// </summary>
  /// <param name="browserId">The source browser id</param>
  /// <param name="frameId">The source frame id</param>
  /// <param name="method">The requested method name</param>
  /// <param name="arguments">The argument list</param>
  virtual void onInvokeMethodNotify(int browserId,
                                    int frameId,
                                    const QString& method,
                                    const QVariantList& arguments) override;

private:
  QRegion draggableRegion_;
  QRegion nonDraggableRegion_;
};

#endif // CUSTOMCEFVIEW_H
