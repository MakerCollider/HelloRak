package org.mc.rak;

import java.util.Arrays;

import android.util.Log;
import android.os.Bundle;
import android.app.Activity;
import android.os.Environment;
import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.net.TrafficStats;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.DialogInterface;

import com.demo.sdk.Enums;
import com.demo.sdk.Module;
import com.demo.sdk.Player;
import com.demo.sdk.Scanner;
import com.demo.sdk.Controller;
import com.demo.sdk.DisplayView;

public class RakVideo extends org.qtproject.qt5.android.bindings.QtActivity {
    private static Module _module;
    private static Player _player;
    private static RakSend _rakSend;
    private static RakVideo _instance;

    public RakVideo() {
        _instance = this;
    }

    public static void Initialize(String s) {
    if (_module == null) {
        _module = new Module(_instance);
    } else {
        _module.setContext(_instance);
    }

        _module.setPlayerPort(554);
        _module.setUsername("admin");
        _module.setPassword("admin");
        _module.setModuleIp("192.168.100.1");
        _module.setLogLevel(Enums.LogLevel.VERBOSE);

        _player = _module.getPlayer();
        _player.setTimeout(10000);
        _player.setAudioOutput(false);
        _player.setRecordFrameRate(20);

        boolean _recording = _player.isRecording();

        _player.setOnGetYUVDataListener(new Player.OnGetYUVDataListener() {
                @Override
                public void onResult(int width, int height,
                                     byte[] yData, byte[] uData, byte[] vData) {
                    _rakSend.sendYUVtoQt(width, height, yData, uData, vData);
                }
        });

        _player.startGetYUVData(true);
        _player.play(Enums.Pipe.H264_PRIMARY, Enums.Transport.UDP);
    }
}
