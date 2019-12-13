#ifndef AUDITIV_RITMUSTARTAS_OBOE_ONMUSICENDEDLISTENER_H
#define AUDITIV_RITMUSTARTAS_OBOE_ONMUSICENDEDLISTENER_H

#include <jni.h>
#include "../utils/logging_macros.h"
#include "../native-lib.h"

class MusicChangedListener {
public:
    virtual void onMusicStarted() = 0;
    virtual void onMusicEnded() = 0;
    virtual ~MusicChangedListener() = default;

    void onMusicEndedCallback() {
        musicEnded();
    }
    void onMusicStartedCallback() {
        musicStarted();
    }
};

#endif //AUDITIV_RITMUSTARTAS_OBOE_ONMUSICENDEDLISTENER_H
