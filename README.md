# KotlinJNICallbackDemo

This demo app demonstrates calling C++ functions from Kotlin and then calling Kotlin functions from C++.

Sound files are played using [Oboe library](https://github.com/google/oboe) version 1.3.0. The Kotlin functions `onMusicStarted` is called from C++ code when the first audio frame is rendered, and `onMusicEnded` is called when the last audio frame is rendered of the sound file. [See Player.cpp](https://github.com/bargergo/KotlinJNICallbackDemo/blob/master/app/src/main/cpp/audio/Player.cpp)

## References
* https://yalantis.com/blog/android-ndk-the-interaction-between-kotlin-and-c-c-plus-plus/
* https://sudonull.com/post/12419-Implementing-the-Observer-Subscriber-pattern-using-JNI-callbacks-on-Android-NDK
* https://github.com/googlearchive/android-audio-high-performance/blob/master/debug-utils/logging_macros.h
* https://github.com/google/oboe/tree/master/samples/RhythmGame
* https://medium.com/@donturner/using-ffmpeg-for-faster-audio-decoding-967894e94e71
