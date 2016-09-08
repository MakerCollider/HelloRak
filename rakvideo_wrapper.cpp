#include <QDebug>
#include <QImage>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

#include "rakvideo_wrapper.h"

QImage RakVideoWrapper::image_;
ImageSignal* RakVideoWrapper::signal_class;

RakVideoWrapper::RakVideoWrapper(QObject *parent) : QObject(parent) {

}

void RakVideoWrapper::sendYUVtoQt(JNIEnv *env, jobject thiz, jint width, jint height,
                                  jbyteArray yData, jbyteArray uData, jbyteArray vData) {

    Q_UNUSED(thiz)

    float yvalue, uvalue, vvalue;

    jbyte* yRawData = env->GetByteArrayElements(yData, 0);
    jbyte* uRawData = env->GetByteArrayElements(uData, 0);
    jbyte* vRawData = env->GetByteArrayElements(vData, 0);

    uchar* cyData = (uchar*)(yRawData);
    uchar* cuData = (uchar*)(uRawData);
    uchar* cvData = (uchar*)(vRawData);

    int r,g,b;
    QImage image = QImage((int)(width), (int)(height), QImage::Format_ARGB32);

    for(int i=0; i<height; i++) {
      for(int j=0; j<width; j++) {
        yvalue = cyData[width*i+j];
        uvalue = cuData[i/2*(width/2)+j/2];
        vvalue = cvData[i/2*(width/2)+j/2];

        r = 1.164f * (yvalue - 16) + 1.596f * (vvalue - 128);
        g = 1.164f * (yvalue - 16) - 0.813f * (vvalue - 128) - 0.391f * (uvalue - 128);
        b = 1.164f * (yvalue - 16) + 2.018f * (uvalue - 128);

        if(r < 0) { r = 0; }
        if(g < 0) { g = 0; }
        if(b < 0) { b = 0; }

        if(r > 254) { r = 254; }
        if(g > 254) { g = 254; }
        if(b > 254) { b = 254; }

        image.setPixel(j,i,qRgb(r,g,b));
      }
    }

    emit signal_class->SignalImage(image);
}

void RakVideoWrapper::registerNativeMethods() {
    QAndroidJniEnvironment env;
    QAndroidJniObject javaClass("org/mc/rak/RakSend");
    jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
    JNINativeMethod methods[] = {{"sendYUVtoQt", "(II[B[B[B)V", (void *)(RakVideoWrapper::sendYUVtoQt)}};

    if(objectClass) {
        env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(objectClass);
    }

    if(env->ExceptionCheck()) {
        env->ExceptionClear();
    }
}

void RakVideoWrapper::callRakJava(QString message) {
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(message);
    QAndroidJniObject::callStaticMethod<void>("org/mc/rak/RakVideo", "Initialize", "(Ljava/lang/String;)V",
                                              javaNotification.object<jstring>());
}
