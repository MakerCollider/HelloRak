#ifndef DRAW_IMAGE_ITEM_H_
#define DRAW_IMAGE_ITEM_H_

#include <QImage>
#include <QQueue>
#include <QQuickPaintedItem>

class QDrawImageItem : public QQuickPaintedItem {
    Q_OBJECT
public:
    QDrawImageItem(QQuickItem *parent = 0);

    void paint(QPainter *painter);

    Q_INVOKABLE void changeImage();
    Q_INVOKABLE void addImageQueue(QImage& image);
private:
    QQueue<QImage> image_queue_;
};

#endif // DRAW_IMAGE_ITEM_H_
