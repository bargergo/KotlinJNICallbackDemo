#include "ObserverChain.h"

ObserverChain::ObserverChain(jweak listener, jmethodID onMusicStarted, jmethodID onMusicEnded)
    :listener(listener), onMusicStarted(onMusicStarted), onMusicEnded(onMusicEnded) {
}