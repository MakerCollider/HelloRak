#include <QTime>
#include <QDebug>
#include <QtConcurrent>

#include "rakvideo_wrapper.h"

ImageSignal* RakVideoWrapper::signal_class;

RakVideoWrapper::RakVideoWrapper(QObject *parent) : QObject(parent) {

}

void RakVideoWrapper::subYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData,
                                  QImage *image, int index) {
  int r,g,b, start, end;
  float yvalue, uvalue, vvalue;

  start = index * (IMAGE_HEIGHT / THREAD_NUMBER);
  end = IMAGE_HEIGHT - (THREAD_NUMBER - index - 1) * (IMAGE_HEIGHT / THREAD_NUMBER);

  for(int i=start; i<end; i++) {
    for(int j=0; j<IMAGE_WIDTH; j++) {
    
      // 图像采用YUV420的方式储存，所以4个（上下左右）Y数据，对应一组UV数据
      yvalue = yData[IMAGE_WIDTH*i+j];
      uvalue = uData[i/2*(IMAGE_WIDTH/2)+j/2];
      vvalue = vData[i/2*(IMAGE_WIDTH/2)+j/2];

      // YUV转换至RGB，判断语句用于修正数值过大或者过小的颜色
      r = 1.164f * (yvalue - 16) + 1.596f * (vvalue - 128);
      g = 1.164f * (yvalue - 16) - 0.813f * (vvalue - 128) - 0.391f * (uvalue - 128);
      b = 1.164f * (yvalue - 16) + 2.018f * (uvalue - 128);

      if(r < 0) { r = 0; }
      if(g < 0) { g = 0; }
      if(b < 0) { b = 0; }

      if(r > 254) { r = 254; }
      if(g > 254) { g = 254; }
      if(b > 254) { b = 254; }

      image->setPixel(j,i,qRgb(r,g,b));
    }
  }
}

void RakVideoWrapper::doYUVtoRGB(QByteArray yData, QByteArray uData, QByteArray vData) {
//  int time_ms;
//  QTime realTime;
//  realTime = QTime::currentTime();
//  qWarning() << "Thread start:" << realTime.toString("hh:mm:ss.zzz");

  QImage image(640,480, QImage::Format_RGB32);

#if 0
  int r,g,b;
  float yvalue, uvalue, vvalue;

  for(int i=0; i<IMAGE_HEIGHT; i++) {
    for(int j=0; j<IMAGE_WIDTH; j++) {
      yvalue = yData[IMAGE_WIDTH*i+j];
      uvalue = uData[i/2*(IMAGE_WIDTH/2)+j/2];
      vvalue = vData[i/2*(IMAGE_WIDTH/2)+j/2];

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
#else
  QFuture<void> future[THREAD_NUMBER];

  // 将图片水平切成THREAD_NUMBER个图片，多线程处理，加快运算速度
  for(int i=0; i< THREAD_NUMBER; i++) {
    future[i] = QtConcurrent::run(subYUVtoRGB, yData, uData, vData, &image, i);
  }

  // 判断所有的图片片段处理转换完成
  while(true) {
    int isFinished = true;
    for(int i=0; i<THREAD_NUMBER; i++) {
      if(!future[i].isFinished()) {
        isFinished = false;
      }
    }

    if(isFinished) {
      break;
    }
  }
#endif

  // 触发信号，传送到QML端进行显示
  emit signal_class->SignalImage(image);

//  time_ms = realTime.msecsTo(QTime::currentTime());
//  qWarning() << "Thread done:" << realTime.toString("hh:mm:ss.zzz") << "useage: " << time_ms;
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

  // 将图像处理部分丢入单独的线程中处理
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
