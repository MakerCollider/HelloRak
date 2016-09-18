#ifndef DRAW_IMAGE_ITEM_H_
#define DRAW_IMAGE_ITEM_H_

#include <QImage>
#include <QQuickPaintedItem>

class QDrawImageItem : public QQuickPaintedItem {
    Q_OBJECT
public:
    QDrawImageItem(QQuickItem *parent = 0);

    void paint(QPainter *painter);
    Q_INVOKABLE void changeImage(QImage& image);
private:
    QImage image123;
};

#endif // DRAW_IMAGE_ITEM_H_
