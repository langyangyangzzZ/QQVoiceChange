package com.qq.voicechange;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.SeekBar;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import com.derry.derry_voicechange.R;
import com.derry.derry_voicechange.databinding.QQBinDing;

import org.fmod.FMOD;

// TODO 终点：本节课目标：Android NDK 导入 C库，开发流程，一劳永逸，任何C库 都是一样的 非常慢
public class MainActivity extends AppCompatActivity implements SeekBar.OnSeekBarChangeListener {

    private static final int MODE_NORMAL = 0; // 正常
    private static final int MODE_LUOLI = 1; //
    private static final int MODE_DASHU = 2; //
    private static final int MODE_JINGSONG = 3; //
    private static final int MODE_GAOGUAI = 4; //
    private static final int MODE_KONGLING = 5; //

    static {
        System.loadLibrary("native-lib");
    }

    //自定义音频
    public native void showCustomVoice(String path, int yindiao, int huiying, int shuaijian, int candou, int sudu);


    private String path;
    private QQBinDing binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = DataBindingUtil.setContentView(this, R.layout.activity_main);

        setTitle("JNI QQ 语音");

        path =  "file:///android_asset/voice.mp3";

        FMOD.init(this);


        binding.setBean1(new QQBean(2, 0, 0, "音调"));
//        10-5000
        binding.setBean2(new QQBean(50000, 10, 10, "回音"));
        ////声音衰减(默认 50 范围:10-100)
        binding.setBean3(new QQBean(100, 10, 10, "声音衰减"));
        //颤抖声音(默认 5  范围:0-20)
        binding.setBean4(new QQBean(20, 0, 0, "声音颤抖"));
        //音速度
        binding.setBean5(new QQBean(2, 0, 1, "声音速度"));

        binding.seekBar1.setOnSeekBarChangeListener(this);
        binding.seekBar2.setOnSeekBarChangeListener(this);
        binding.seekBar3.setOnSeekBarChangeListener(this);
        binding.seekBar4.setOnSeekBarChangeListener(this);
        binding.seekBar5.setOnSeekBarChangeListener(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    // 六个 点击事件
    public void onFix(View view) {
        switch (view.getId()) {
            case R.id.btn_normal:
                new Thread(() -> voiceChangeNative(MODE_NORMAL, path)).start();
                break;
            case R.id.btn_luoli:
                new Thread(() -> voiceChangeNative(MODE_LUOLI, path)).start();
                break;
            case R.id.btn_dashu:
                new Thread(() -> voiceChangeNative(MODE_DASHU, path)).start();
                break;
            case R.id.btn_jingsong:
                new Thread(() -> voiceChangeNative(MODE_JINGSONG, path)).start();
                break;
            case R.id.btn_gaoguai:
                new Thread(() -> voiceChangeNative(MODE_GAOGUAI, path)).start();
                break;
            case R.id.btn_kongling:
                new Thread(() -> voiceChangeNative(MODE_KONGLING, path)).start();
                break;
        }
    }

    // 给C++调用的函数
    // JNI 调用 Java函数的时候，忽略掉 私有、公开 等
    private void playerEnd(String msg) {
        runOnUiThread(
                () -> Toast.makeText(MainActivity.this, "" +msg, Toast.LENGTH_SHORT).show()
        );
    }

    private native void voiceChangeNative(int modeNormal, String path);

    //播放按钮
    public void onClick7(View view) {
        Log.i("onClick7",
                binding.seekBar1.getProgress()+"\t"+
                        binding.seekBar2.getProgress()+"\t"+
                        binding.seekBar3.getProgress()+"\t"+
                        binding.seekBar4.getProgress()+"\t"+
                        binding.seekBar5.getProgress());

        new Thread(() -> showCustomVoice(path,
                binding.seekBar1.getProgress(),
                binding.seekBar2.getProgress(),
                binding.seekBar3.getProgress(),
                binding.seekBar4.getProgress(),
                binding.seekBar5.getProgress()
        )).start();
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        switch (seekBar.getId()) {
            case R.id.seekBar1:
                binding.setBean1(new QQBean(2, 0, progress, "音调"));
                break;
            case R.id.seekBar2:
//        10-5000
                binding.setBean2(new QQBean(50000, 10, progress, "回音"));
                break;
            case R.id.seekBar3:
                ////声音衰减(默认 50 范围:10-100)
                binding.setBean3(new QQBean(100, 10, progress, "声音衰减"));
                break;
            case R.id.seekBar4:
                //颤抖声音(默认 5  范围:0-20)
                binding.setBean4(new QQBean(20, 0, progress, "声音颤抖"));
                break;
            case R.id.seekBar5:
                //音速度
                binding.setBean5(new QQBean(2, 0, progress, "声音速度"));
                break;
            default:
                throw new IllegalStateException("Unexpected value: " + seekBar.getId());
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }
}
