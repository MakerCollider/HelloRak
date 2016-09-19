#ifndef RAKVIDEO_WRAPPER_H_
#define RAKVIDEO_WRAPPER_H_

#include <QImage>
#include <QQueue>
#include <QObject>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

#include "image_signal.h"

class RakVideoWrapper : public QObject {
    Q_OBJECT
public:
    explicit RakVideoWrapper(QObject *parent = 0);
    static void doYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData);
    static void sendYUVtoQt(JNIEnv *env, jobject thiz, jint width, jint height, jbyteArray yData,
                     jbyteArray uData, jbyteArray vData);

    static void registerNativeMethods();

    Q_INVOKABLE void callRakJava(QString message);

    static ImageSignal *signal_class;

    static QImage image11;
};

#endif // RAKVIDEO_WRAPPER_H_
