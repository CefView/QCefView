#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QCefView.h>

class CustomCefView : public QCefView
{
  Q_OBJECT

public:
  CustomCefView(const QString url, QWidget* parent = 0);

  ~CustomCefView();

  void changeColor();

protected:
    
//#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
//  virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
//#else
//  virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
//#endif
    
  virtual void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion) override;

  virtual void onQCefUrlRequest(const QString& url) override;

  virtual void onQCefQueryRequest(const QCefQuery& query) override;

  virtual void onInvokeMethodNotify(int browserId,
                                    int frameId,
                                    const QString& method,
                                    const QVariantList& arguments) override;

private:
  QRegion draggableRegion_;
  QRegion nonDraggableRegion_;
};

#endif // CUSTOMCEFVIEW_H
