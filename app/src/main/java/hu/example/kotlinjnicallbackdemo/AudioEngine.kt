package hu.example.kotlinjnicallbackdemo

import android.content.res.AssetManager

object AudioEngine {
    external fun initEngine(assetManager: AssetManager)
    external fun startEngine()
    external fun stopEngine()
    external fun clap()
    external fun loadClap(filename: String)
    external fun loadBackingTrack(filename: String)
    external fun playBackingTrack()
    external fun stopBackingTrack()
    external fun subscribe(listener: MusicChangedListener)
    external fun deleteSubscriptions()

    init {
        System.loadLibrary("native-lib")
    }

    interface MusicChangedListener {
        fun onMusicStarted()
        fun onMusicEnded()
    }
}