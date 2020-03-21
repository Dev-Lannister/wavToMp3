package com.youdao.lame;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.Toast;

import com.youdao.lame.util.ConvertUtil;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final ConvertUtil convertUtil = new ConvertUtil();
        convertUtil.setListener(new ConvertUtil.onConvertListener() {
            @Override
            public void showMsg() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "convert complete", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });
        new Thread(new Runnable() {
            @Override
            public void run() {
                File file = new File("/sdcard/test.mp3");
                if (file.exists()) file.delete();
                convertUtil.convertmp3("/sdcard/test.wav", "/sdcard/test.mp3", 16000, 1);
            }
        }).start();

    }


}
