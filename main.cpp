#include <QDebug>
#include <QMetaType>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVariant>

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

    ctx->setContextProperty("rak",&rak);
    ctx->setContextProperty("signal_class",&signal_class);

    qRegisterMetaType<QImage>("QImage&");
    qmlRegisterType<QDrawImageItem>("DrawItems", 1, 0, "QDrawImageItem");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
