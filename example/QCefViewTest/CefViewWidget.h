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

signals:
  void newDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName);

  void updateDownloadItem(const QSharedPointer<QCefDownloadItem>& item);

protected:
  bool onBeforePopup(qint64 frameId,
                     const QString& targetUrl,
                     const QString& targetFrameName,
                     QCefView::CefWindowOpenDisposition targetDisposition,
                     QRect& rect,
                     QCefSetting& settings) override;

  void onNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName) override;

  void onUpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item) override;

  void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

protected slots:
  void onScreenChanged(QScreen* screen);

  void onNativeBrowserWindowCreated(QWindow* window);

protected:
  virtual void resizeEvent(QResizeEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

private:
  void updateMask();

private:
  QWindow* m_pCefWindow = nullptr;

  int m_iCornerRadius = 50;

  QRegion m_draggableRegion;

  QRegion m_nonDraggableRegion;
};

#endif // CUSTOMCEFVIEW_H
