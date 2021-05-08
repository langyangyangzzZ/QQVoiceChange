#include <unistd.h>
#include "com_qq_voicechange_MainActivity.h"

using namespace FMOD; // fmod的命名空间

extern "C" JNIEXPORT void JNICALL Java_com_qq_voicechange_MainActivity_voiceChangeNative
        (JNIEnv * env, jobject thiz, jint mode, jstring path) {


    char *content = "播放完毕";

    //接收传过来的路径
    const char *path_ = env->GetStringUTFChars(path, nullptr);

    //音效引擎系统
    System *system = nullptr;

    //声音
    Sound *sound = nullptr;

    //声音通道(音轨)
    Channel *channel = nullptr;

    //DSP (digital signal process 数字信号处理)
    //音效的核心通过这个来改变
    DSP *dsp = nullptr;

    //TODO 第一步,创建系统
    System_Create(&system);

    //TODO 第二步,系统初始化
    /**
     * 参数一:通道数(这里随便取正数)
     * 参数二:宏定义初始化系统
     * 参数三:(不认识)
     */
    system->init(32, FMOD_INIT_NORMAL, nullptr);

    //TODO 第三步:创建声音
    /**
     * 参数一:声音播放路径
     * 参数二:声音初始化标记
     * 参数三:额外数据(不认识)
     * 参数四:声音
     */
    system->createSound(path_, FMOD_DEFAULT, nullptr, &sound);

    //TODO 第四步:播放声音
    /**
     * 参数一:声音
     * 参数二:音轨分组(0 代表不分组)
     * 参数三:控制(不认识)
     * 参数四:通道
     */
    system->playSound(sound, nullptr, false, &channel);


    // TODO 第五步：增加特效
    switch (mode) {
        case com_derry_derry_voicechange_MainActivity_MODE_NORMAL: // 原生
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_LUOLI: // 萝莉

            // 音调高 -- 萝莉 2.0
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_DASHU: // 大叔

            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp);
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_GAOGUAI: // 搞怪

            // 小黄人声音 频率快

            // 从音轨拿 当前 频率
            float mFrequency;
            channel->getFrequency(&mFrequency);

            // 修改频率
            channel->setFrequency(mFrequency * 1.5f); // 频率加快  小黄人的声音
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_JINGSONG: // 惊悚

            // 惊悚音效：特点： 很多声音的拼接

            // TODO 音调低
            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch 音调条件
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效进去 音轨
            channel->addDSP(0, dsp); // 第一个音轨

            // TODO 搞点回声
            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(1,dsp); // 第二个音轨

            // TODO 颤抖
            // Tremolo 颤抖音 正常5    非常颤抖  20
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20); // 非常颤抖
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f); // ？？？
            channel->addDSP(2, dsp); // 第三个音轨

            // 调音师：才能跳出来  同学们自己去调
            break;
        case com_derry_derry_voicechange_MainActivity_MODE_KONGLING: // 空灵  学校广播

            // 回音 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时    to 5000.  Default = 500.
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度 Default = 50   0 完全衰减了
            channel->addDSP(0,dsp);
            break;

    }

    //TODO 播放完毕
    bool is_play = true;
    while (is_play) {
        //是否播放完成,音轨最清楚,如果播放完成音轨会将 is_play 修改成 false
        channel->isPlaying(&is_play);

        //每隔一秒睡眠一次
        usleep(1000 * 1000);
    }

    //TODO 一定要回收
    sound->release();
    system->close();
    system->release();
    sound = nullptr;
    system = nullptr;
    env->ReleaseStringUTFChars(path, path_);

    // 告知Java播放完毕
    jclass mainCls = env->GetObjectClass(thiz);
    jmethodID endMethod = env->GetMethodID(mainCls, "playerEnd", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(content);
    env->CallVoidMethod(thiz, endMethod, value);
}

//自定义音频
extern "C"
JNIEXPORT void JNICALL
Java_com_qq_voicechange_MainActivity_showCustomVoice(JNIEnv *env, jobject thiz,
                                                               jstring path, jint yindiao,
                                                               jint huiying, jint shuaijian,
                                                               jint candou, jint sudu){
    //接收传过来的路径
    const char *path_ = env->GetStringUTFChars(path, nullptr);

    //音效引擎系统
    System *system = nullptr;

    //声音
    Sound *sound = nullptr;

    //声音通道(音轨)
    Channel *channel = nullptr;

    //DSP (digital signal process 数字信号处理)
    //音效的核心通过这个来改变
    DSP *dsp = nullptr;

    //TODO 第一步,创建系统
    System_Create(&system);


    //TODO 第二步,系统初始化
    /**
     * 参数一:通道数(这里随便取正数)
     * 参数二:宏定义初始化系统
     * 参数三:(不认识)
     */
    system->init(32, FMOD_INIT_NORMAL, nullptr);

    //TODO 第三步:创建声音
    /**
     * 参数一:声音播放路径
     * 参数二:声音初始化标记
     * 参数三:额外数据(不认识)
     * 参数四:声音
     */
    system->createSound(path_, FMOD_DEFAULT, nullptr, &sound);

    //TODO 第四步:播放声音
    /**
     * 参数一:声音
     * 参数二:音轨分组(0 代表不分组)
     * 参数三:控制(不认识)
     * 参数四:通道
     */
    system->playSound(sound, nullptr, false, &channel);


    //=======  调节音调  ======
    system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
    dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, yindiao);
    channel->addDSP(0, dsp);

    //=======  回音  ======
    //FMOD_DSP_TYPE_ECHO回音
    system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
    //回音 (默认 500  范围:10-5000)
    dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, huiying);
    //声音衰减(默认 50 范围:10-100)
    dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, shuaijian);
    //添加到通道
    channel->addDSP(1, dsp);

    //=======  声音颤抖  ======
    //FMOD_DSP_TYPE_TREMOLO 颤抖声音(默认 5  范围:0-20)
    system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
    dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, candou);
    //添加到通道
    channel->addDSP(2, dsp);

    float frequency = 0;
    //从音轨拿频率
    channel->getFrequency(&frequency);

    //调节频率
    channel->setFrequency(frequency * sudu);

    //TODO 播放完毕
    bool is_play = true;
    while (is_play) {
        //是否播放完成,音轨最清楚,如果播放完成音轨会将 is_play 修改成 false
        channel->isPlaying(&is_play);

        //每隔一秒睡眠一次
        usleep(1000 * 1000);
    }

    //TODO 一定要回收
    sound->release();
    system->close();
    system->release();
    sound = nullptr;
    system = nullptr;
    env->ReleaseStringUTFChars(path, path_);

}