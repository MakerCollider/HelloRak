#include <QDebug>
#include <QTime>
#include <QtConcurrent>

#include "rakvideo_wrapper.h"

ImageSignal* RakVideoWrapper::signal_class;

RakVideoWrapper::RakVideoWrapper(QObject *parent) : QObject(parent) {

}

void RakVideoWrapper::doYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData) {
    int r,g,b;
    float yvalue, uvalue, vvalue;

    int width = 640;
    int height = 480;

    QImage image(640,480, QImage::Format_RGB32);

//    QTime realTime = QTime::currentTime();
//    qWarning() << "Thread start:" << realTime.toString("hh:mm:ss.zzz");

    for(int i=0; i<height; i++) {
      for(int j=0; j<width; j++) {
        yvalue = yData[width*i+j];
        uvalue = uData[i/2*(width/2)+j/2];
        vvalue = vData[i/2*(width/2)+j/2];

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

//    int time_ms = realTime.msecsTo(QTime::currentTime());
//    qWarning() << "Thread done:" << realTime.toString("hh:mm:ss.zzz") << "useage: " << time_ms;
    emit signal_class->SignalImage(image);
}

void RakVideoWrapper::sendYUVtoQt(JNIEnv *env, jobject thiz, jint width, jint height,
                                  jbyteArray yData, jbyteArray uData, jbyteArray vData) {
    Q_UNUSED(thiz)

    jbyte* yRawData = env->GetByteArrayElements(yData, 0);
    jbyte* uRawData = env->GetByteArrayElements(uData, 0);
    jbyte* vRawData = env->GetByteArrayElements(vData, 0);

    char* cyData = (char*)(yRawData);
    char* cuData = (char*)(uRawData);
    char* cvData = (char*)(vRawData);

    QByteArray yArray(cyData, width*height);
    QByteArray uArray(cuData, width*height/4);
    QByteArray vArray(cvData, width*height/4);

    QFuture<void> future = QtConcurrent::run(doYUVtoRGB, yArray, uArray, vArray);
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
