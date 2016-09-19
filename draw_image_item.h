#ifndef DRAW_IMAGE_ITEM_H_
#define DRAW_IMAGE_ITEM_H_

#include <QImage>
#include <QQueue>
#include <QQuickPaintedItem>

// 图像绘制类
class QDrawImageItem : public QQuickPaintedItem {
  Q_OBJECT
public:
  QDrawImageItem(QQuickItem *parent = 0);
  
  // 绘制函数
  void paint(QPainter *painter);

  // 更新界面图像
  Q_INVOKABLE void changeImage();
  
  // 图像队列压入图像
  Q_INVOKABLE void addImageQueue(QImage& image);
private:
  // 待显示图像队列
  QQueue<QImage> image_queue_;
};

#endif // DRAW_IMAGE_ITEM_H_
