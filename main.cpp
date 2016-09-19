#include <QDebug>
#include <QVariant>
#include <QMetaType>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "image_signal.h"
#include "draw_image_item.h"
#include "rakvideo_wrapper.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  ImageSignal signal_class;

  RakVideoWrapper rak;
  RakVideoWrapper::signal_class = &signal_class;

  rak.registerNativeMethods();

  QQmlApplicationEngine engine;
  QQmlContext *ctx = engine.rootContext();

  // 将C++函数导入到QML中
  ctx->setContextProperty("rak",&rak);
  ctx->setContextProperty("signal_class",&signal_class);

  // 在QML中注册自定义类型，自定义控件
  qRegisterMetaType<QImage>("QImage&");
  qmlRegisterType<QDrawImageItem>("DrawItems", 1, 0, "QDrawImageItem");

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}
