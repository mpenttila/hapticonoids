package fi.hapticonoids;

import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Handler;
import android.os.Looper;

/**
 * This class implements thread which will play the sound effects from the player.
 * @author Veli-Pekka Kestil�, Markus Penttil�
 *
 */
public class SoundThread extends Thread implements MessageEater {

	Handler msgQueue = null;
	private static int type = 1;
	Hapticonoids hapticonoids = null;
	SoundPool sounds = null;
	int[] soundsIndex = new int[6];

	public SoundThread(Hapticonoids hapticonoids) {
		this.hapticonoids = hapticonoids;
		this.sounds = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);
		soundsIndex[1] = this.sounds.load(hapticonoids, R.raw.thud2, 1);
		soundsIndex[2] = this.sounds.load(hapticonoids, R.raw.boink2, 1);
		soundsIndex[3] = this.sounds.load(hapticonoids, R.raw.siren_loop, 1);
		soundsIndex[4] = this.sounds.load(hapticonoids, R.raw.zoom, 1);
		soundsIndex[5] = this.sounds.load(hapticonoids, R.raw.beep7 , 1);
	}

	@Override
	public void run() {
		System.out.println("Starting prepare");
		Looper.prepare();
		System.out.println("Creating handler");
		this.msgQueue = new Handler();
		System.out.println("Starting loop");
        Looper.loop();
	}	
	
	/**
	 * Get the type of the message eater. (1 sound processor)
	 */
	public int getType() {
		return SoundThread.type;
	}

	/**
	 * Stop the sound processing loop.
	 */
	public synchronized void stopListener() {
		this.msgQueue.post(new Runnable() {
			public void run() {
				Looper.myLooper().quit();				
			}
		});
	}
	
	/**
	 * Play the sound effect.
	 */
	public synchronized void doTask(int id) {
		this.sounds.play(this.soundsIndex[id], (float)1.0, (float)1.0, 10, 0, (float)1.0);
	}

}
