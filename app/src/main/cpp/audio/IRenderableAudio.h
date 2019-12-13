#ifndef AUDITIV_RITMUSTARTAS_OBOE_IRENDERABLEAUDIO_H
#define AUDITIV_RITMUSTARTAS_OBOE_IRENDERABLEAUDIO_H

#include <cstdint>
#include <string>

class IRenderableAudio {

public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float *audioData, int32_t numFrames) = 0;
};

#endif //AUDITIV_RITMUSTARTAS_OBOE_IRENDERABLEAUDIO_H
