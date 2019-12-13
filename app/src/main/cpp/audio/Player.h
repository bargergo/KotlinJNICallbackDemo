#ifndef AUDITIV_RITMUSTARTAS_OBOE_PLAYER_H
#define AUDITIV_RITMUSTARTAS_OBOE_PLAYER_H

#include <cstdint>
#include <array>

#include <chrono>
#include <atomic>
#include <utility>
#include <android/asset_manager.h>

#include "IRenderableAudio.h"
#include "DataSource.h"
#include "MusicChangedListener.h"

class Player : public IRenderableAudio {

public:
    /**
     * Construct a new Player from the given DataSource. Players can share the same data source.
     * For example, you could play two identical sounds concurrently by creating 2 Players with the
     * same data source.
     *
     * @param source
     */
    Player(std::shared_ptr<DataSource> source)
            : mSource(source)
    {};

    Player(std::shared_ptr<Player> player)
            : mSource{player->mSource} {

    };

    void renderAudio(float *targetData, int32_t numFrames);
    void resetPlayHead() { mReadFrameIndex = 0; };
    void setPlaying(bool isPlaying) { mIsPlaying = isPlaying; resetPlayHead(); };
    void setLooping(bool isLooping) { mIsLooping = isLooping; };
    void setOnMusicEndedListener(MusicChangedListener *);

private:
    int32_t mReadFrameIndex = 0;
    std::atomic<bool> mIsPlaying { false };
    std::atomic<bool> mIsLooping { false };
    std::shared_ptr<DataSource> mSource;
    MusicChangedListener * musicChangedListener{nullptr};

    void renderSilence(float*, int32_t);
};


#endif //AUDITIV_RITMUSTARTAS_OBOE_PLAYER_H
