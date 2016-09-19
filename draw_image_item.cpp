#include <QDebug>
#include <QPainter>

#include "draw_image_item.h"

QDrawImageItem::QDrawImageItem(QQuickItem *parent) : QQuickPaintedItem(parent) {

}

void QDrawImageItem::paint(QPainter *painter) {
  if(!image_queue_.empty()) {
    painter->drawImage(0,0,image_queue_.dequeue());
  } else {
      qWarning("image_queue_ is empty");
  }
}

void QDrawImageItem::changeImage() {
    if(!image_queue_.empty()) {
      update();
    }
    //qWarning() << "changeImage";
}

void QDrawImageItem::addImageQueue(QImage& image) {
    image_queue_.enqueue(image);
    //qWarning() << "addImageQueue";
}
