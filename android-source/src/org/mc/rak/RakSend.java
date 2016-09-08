package org.mc.rak;

import android.util.Log;
public class RakSend {
    public static native void sendYUVtoQt(int width, int height, byte[] yData, byte[] uData, byte[] vData);
}
