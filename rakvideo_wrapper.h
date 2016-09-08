#ifndef RAKVIDEO_WRAPPER_H_
#define RAKVIDEO_WRAPPER_H_

#include <QObject>
#include <signal.h>
#include <QImage>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include "image_signal.h"

class RakVideoWrapper : public QObject {
    Q_OBJECT
public:
    explicit RakVideoWrapper(QObject *parent = 0);
    Q_INVOKABLE void callRakJava(QString message);

    static void sendYUVtoQt(JNIEnv *env, jobject thiz, jint width, jint height, jbyteArray yData,
                     jbyteArray uData, jbyteArray vData);

    static void registerNativeMethods();

    static ImageSignal *signal_class;

    static QImage image_;

signals:
    void getImage();
};

#endif // RAKVIDEO_WRAPPER_H_
