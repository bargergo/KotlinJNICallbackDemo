#ifndef MYJNACALLBACKTEST_OBSERVERCHAINFACTORY_H
#define MYJNACALLBACKTEST_OBSERVERCHAINFACTORY_H


#include <jni.h>

class ObserverChain {
public:
    ObserverChain(jweak listener, jmethodID onMusicStarted, jmethodID onMusicEnded);

    jweak listener = nullptr;
    jmethodID onMusicStarted = nullptr;
    jmethodID onMusicEnded = nullptr;

};


#endif //MYJNACALLBACKTEST_OBSERVERCHAINFACTORY_H