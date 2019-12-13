#ifndef RITMUSTARTAS_AUDIOENGINE_H
#define RITMUSTARTAS_AUDIOENGINE_H

#include <string>
#include <memory>
#include <android/asset_manager.h>
#include <oboe/Oboe.h>
#include <map>
#include "audio/Mixer.h"
#include "audio/Player.h"
#include "audio/MusicChangedListener.h"


using namespace oboe;

class AudioEngine : public AudioStreamCallback, public MusicChangedListener {
public:
    explicit AudioEngine(AAssetManager * assetManager): mAssetManager(assetManager) {}
    void start();
    void init();
    void stop();
    void clap();
    void loadClap(const std::string& filename);
    void loadBackingTrack(const std::string& filename);
    void playBackingTrack();
    void stopBackingTrack();
    void onMusicEnded() override;
    void onMusicStarted() override;

    // Inherited from oboe::AudioStreamCallback
    DataCallbackResult onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

private:
    std::shared_ptr<Player> loadCompressedAsset(const std::string& filename);
    AAssetManager * mAssetManager{nullptr};
    std::shared_ptr<Player> mClap{nullptr};
    std::shared_ptr<Player> mBackingTrack{nullptr};
    Mixer mMixer;
    AudioStreamBuilder builder;
    std::unique_ptr<float[]> mConversionBuffer { nullptr }; // For float->int16 conversion
    AudioProperties mTargetProperties;
    ManagedStream mAudioStream;
};


#endif //RITMUSTARTAS_AUDIOENGINE_H
