#include <QDebug>
#include <QPainter>

#include "draw_image_item.h"

QDrawImageItem::QDrawImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    //image123 = new QImage(640,480,QImage::Format_RGB32);
}

void QDrawImageItem::paint(QPainter *painter) {

    painter->drawImage(0,0,image123);
}

void QDrawImageItem::changeImage(QImage& image) {
  image123 = image.copy(0,0,image.width(),image.height());
  update();
}
