#include "AudioEngine.h"
#include "utils/logging_macros.h"
#include "GameConstants.h"
#include "audio/AAssetDataSource.h"

void AudioEngine::start() {
    Result result = mAudioStream->requestStart();
    if (result != Result::OK){
        LOGE("Failed to start stream. Error: %s", convertToText(result));
    }
}

void AudioEngine::stop() {
    if (mBackingTrack != nullptr)
        mBackingTrack->setPlaying(false);
    mAudioStream->requestStop();
}

void AudioEngine::clap() {
    mClap->setPlaying(true);
}

DataCallbackResult AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {

    // If our audio stream is expecting 16-bit samples we need to render our floats into a separate
    // buffer then convert them into 16-bit ints
    bool is16Bit = (oboeStream->getFormat() == AudioFormat::I16);
    float *outputBuffer = (is16Bit) ? mConversionBuffer.get() : static_cast<float *>(audioData);

    for (int i = 0; i < numFrames; ++i) {
        mMixer.renderAudio(outputBuffer + (oboeStream->getChannelCount()*i), 1);
    }

    if (is16Bit){
        oboe::convertFloatToPcm16(outputBuffer,
                                  static_cast<int16_t*>(audioData),
                                  numFrames * oboeStream->getChannelCount());
    }

    return DataCallbackResult::Continue;
}

void AudioEngine::init() {

    builder.setFormat(AudioFormat::I16);
    builder.setChannelCount(2);
    builder.setSampleRate(48000);

    builder.setCallback(this);

    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);

    Result result = builder.openManagedStream(mAudioStream);
    if (result != Result::OK){
        LOGE("Failed to open stream. Error: %s", convertToText(result));
    }

    if (mAudioStream->getFormat() == AudioFormat::I16){
        mConversionBuffer = std::make_unique<float[]>(
                (size_t)mAudioStream->getBufferCapacityInFrames() *
                mAudioStream->getChannelCount());
    }

    // Reduce stream latency by setting the buffer size to a multiple of the burst size
    mAudioStream->setBufferSizeInFrames(mAudioStream->getFramesPerBurst() * 2);

    // Set the properties of our audio source(s) to match that of our audio stream
    mTargetProperties = AudioProperties {
            .channelCount = mAudioStream->getChannelCount(),
            .sampleRate = mAudioStream->getSampleRate()
    };
    mMixer.setChannelCount(mAudioStream->getChannelCount());
}

std::shared_ptr<Player> AudioEngine::loadCompressedAsset(const std::string& filename) {
    std::shared_ptr<AAssetDataSource> source {
            AAssetDataSource::newFromCompressedAsset(*mAssetManager, filename.c_str(), mTargetProperties)
    };
    if (source == nullptr){
        LOGE("Could not load source data from \"%s\"", filename.c_str());
        return nullptr;
    }
    return std::make_shared<Player>(source);
}

void AudioEngine::loadClap(const std::string& filename) {
    if (mClap == nullptr) {
        mClap = loadCompressedAsset(filename);
        if (mClap != nullptr) {
            mMixer.addTrack(mClap);
        }
    }
}

void AudioEngine::loadBackingTrack(const std::string& filename) {
    stopBackingTrack();
    if (mBackingTrack != nullptr) {
        mBackingTrack->setOnMusicEndedListener(nullptr);
    }
    mBackingTrack = loadCompressedAsset(filename);

    if (mBackingTrack != nullptr) {
        mBackingTrack->setOnMusicEndedListener(this);
        mMixer.addTrack(mBackingTrack);
    } else {
        LOGE("Backing track is null");
    }
}

void AudioEngine::stopBackingTrack() {
    if (mBackingTrack != nullptr) {
        mBackingTrack->setPlaying(false);
    }
}


void AudioEngine::playBackingTrack() {
    if (mBackingTrack == nullptr) {
        LOGE("AudioEngine::playBackingTrack() mBackingTrack is null");
        return;
    }
    mBackingTrack->setPlaying(true);
}

void AudioEngine::onMusicEnded() {
    LOGD("MusicEnded");
    onMusicEndedCallback();
}

void AudioEngine::onMusicStarted() {
    LOGD("MusicEnded");
    onMusicStartedCallback();
}

