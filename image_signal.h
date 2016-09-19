#ifndef IMAGE_SIGNAL_H_
#define IMAGE_SIGNAL_H_

#include <QObject>

// 用于传送QImage的Signal类
class ImageSignal : public QObject {
  Q_OBJECT
public:
  explicit ImageSignal(QObject *parent = 0);

signals:
  void SignalImage(QImage& image);
};

#endif // IMAGE_SIGNAL_H_
