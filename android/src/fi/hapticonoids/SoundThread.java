package fi.hapticonoids;

import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Handler;
import android.os.Looper;

public class SoundThread extends Thread implements MessageEater {

	Handler msgQueue = null;
	private static int type = 1;
	Hapticonoids hapticonoids = null;
	SoundPool sounds = null;
	int[] soundsIndex = new int[5];

	public SoundThread(Hapticonoids hapticonoids) {
		this.hapticonoids = hapticonoids;
		this.sounds = new SoundPool(1, AudioManager.STREAM_MUSIC, 0);
		soundsIndex[1] = this.sounds.load(hapticonoids, R.raw.thud2, 1);
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
	
	public int getType() {
		return SoundThread.type;
	}

	public synchronized void stopListener() {
		this.msgQueue.post(new Runnable() {
			public void run() {
				Looper.myLooper().quit();				
			}
		});
	}

	private class SoundRunnable implements Runnable {
		int id = -1;
		SoundThread st = null;
		
		public SoundRunnable(SoundThread st, int id) {
			this.id = id;
			this.st = st;
		}
		
		public void run() {
			this.st.sounds.play(this.st.soundsIndex[this.id], (float)1.0, (float)1.0, 10, 0, (float)1.0);
			System.out.println("Thud!");
		}		
	}
	
	public synchronized void doTask(int id) {
		//SoundRunnable sr = new SoundRunnable(this, id);		
		//this.msgQueue.post(sr);
		this.sounds.play(this.soundsIndex[id], (float)1.0, (float)1.0, 10, 0, (float)1.0);
	}

}
