#include <QDebug>
#include <QPainter>

#include "draw_image_item.h"

QDrawImageItem::QDrawImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    image123 = new QImage(640,480,QImage::Format_RGB32);
}

void QDrawImageItem::paint(QPainter *painter) {
    const QImage p = *image123;

    painter->drawImage(0,0,p);

}

void QDrawImageItem::changeImage(QImage& image) {
  image123 = &image;
  update();
}
