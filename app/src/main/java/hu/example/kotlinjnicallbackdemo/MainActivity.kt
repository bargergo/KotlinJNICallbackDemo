package hu.example.kotlinjnicallbackdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import kotlinx.android.synthetic.main.activity_main.*
import java.sql.Timestamp

class MainActivity : AppCompatActivity(), AudioEngine.MusicChangedListener {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        AudioEngine.apply {
            initEngine(assets)
            subscribe(this@MainActivity)
            loadClap("CLAP.mp3")
            loadBackingTrack("FUNKY_HOUSE.mp3")
        }

        btnStart.setOnClickListener {
            AudioEngine.stopBackingTrack()
            tvStartValue.text = getString(R.string.not_set)
            tvEndValue.text = getString(R.string.not_set)
            AudioEngine.playBackingTrack()
        }
        btnClap.setOnClickListener {
            AudioEngine.clap()
        }
    }

    override fun onStart() {
        super.onStart()
        AudioEngine.startEngine()
    }

    override fun onStop() {
        super.onStop()
        AudioEngine.stopEngine()
    }

    override fun onDestroy() {
        super.onDestroy()
        AudioEngine.deleteSubscriptions()
    }

    override fun onMusicStarted() {
        runOnUiThread {
            tvStartValue.text = "${Timestamp(System.currentTimeMillis())}"
        }
    }

    override fun onMusicEnded() {
        runOnUiThread {
            tvEndValue.text = "${Timestamp(System.currentTimeMillis())}"
        }
    }
}
