#ifndef RAKVIDEO_WRAPPER_H_
#define RAKVIDEO_WRAPPER_H_

#include <QImage>
#include <QObject>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

#include "image_signal.h"

#define IMAGE_WIDTH     640
#define IMAGE_HEIGHT    480
#define THREAD_NUMBER   8

class RakVideoWrapper : public QObject {
    Q_OBJECT
public:
    explicit RakVideoWrapper(QObject *parent = 0);
    
    // 图像处理线程
    static void doYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData);
    
    // 图像处理子线程，用于并行处理YUV数据
    static void subYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData,
                            QImage *image, int index);

    // JAVA端callback函数，用于接收YUV数据
    static void sendYUVtoQt(JNIEnv *env, jobject thiz, jint width, jint height,
                            jbyteArray yData,jbyteArray uData, jbyteArray vData);

    static void registerNativeMethods();

    //RakVideo类初始化
    Q_INVOKABLE void callRakJava(QString message);

    //QImage类型信号
    static ImageSignal *signal_class;
};

#endif // RAKVIDEO_WRAPPER_H_
