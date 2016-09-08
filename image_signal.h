#ifndef IMAGE_SIGNAL_H_
#define IMAGE_SIGNAL_H_

#include <QObject>

class ImageSignal : public QObject {
    Q_OBJECT
public:
    explicit ImageSignal(QObject *parent = 0);

signals:
    void SignalImage(QImage& image);
public slots:
};

#endif // IMAGE_SIGNAL_H_
