#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QScreen>

#include <QCefView.h>

/// <summary>
/// Represents the customized QCefView
/// </summary>
class CefViewWidget : public QCefView
{
  Q_OBJECT

public:
  CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent = 0);

  ~CefViewWidget();

protected slots:
  void onScreenChanged(QScreen* screen);

  void onNativeBrowserWindowCreated(QWindow* window);

  void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

protected:
  bool onNewPopup(const QCefFrameId& sourceFrameId,
                  const QString& targetUrl,
                  QString& targetFrameName,
                  QCefView::CefWindowOpenDisposition targetDisposition,
                  QRect& rect,
                  QCefSetting& settings,
                  bool& disableJavascriptAccess) override;

  void onNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName) override;

  void onUpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item) override;

protected:
  void resizeEvent(QResizeEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;

private:
  void updateMask();

private:
  QWindow* m_pCefWindow = nullptr;

  int m_iCornerRadius = 50;

  QRegion m_draggableRegion;

  QRegion m_nonDraggableRegion;
};

#endif // CUSTOMCEFVIEW_H
