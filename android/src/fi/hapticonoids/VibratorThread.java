package fi.hapticonoids;

import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;
import android.util.Log;

/**
 * Class realizing the thread to process vibration events.
 * @author Veli-Pekka Kestilä, Markus Penttilä
 *
 */
public final class VibratorThread extends Thread implements MessageEater {

	Vibrator vibra;
	Handler msgQueue = null;
	private static int type = 0;
	private Hapticonoids hapticonoids;
	
	public static final int[] VIBRATION_LENGTH = {0, 200, 400, 600, 800, 1000};
	
	/**
	 * Constructor accepting the vibration resource and the main activity.
	 * @param vibra
	 * @param hapticonoids
	 */
	public VibratorThread(Vibrator vibra, Hapticonoids hapticonoids) {
		this.vibra = vibra;
		this.hapticonoids = hapticonoids;
	}
	
	/**
	 * Set up the message processor.
	 */
	@Override
	public void run() {
		Log.i("Hapticonoids::VibratorThread","Starting prepare");
		Looper.prepare();
		Log.i("Hapticonoids::VibratorThread","Creating handler");
		this.msgQueue = new Handler();
		Log.i("Hapticonoids::VibratorThread","Starting loop");
        Looper.loop();
	}
	
	/**
	 * Return the type of the thread 0 for vibrator 
	 */
	public int getType() {
		return VibratorThread.type;
	}
	
	/**
	 * Stop listening vibration events.
	 */
	public synchronized void stopListener() {
		this.msgQueue.post(new Runnable() {
			public void run() {
				Looper.myLooper().quit();				
			}
		});
	}

	/**
	 * Process the actual vibration event from Bluetooth client.
	 */
	public synchronized void doTask(final int id) {
		this.msgQueue.post(new Runnable() {
			public void run() {
				hapticonoids.setInfoText("Vibrating!");
				Log.i("Hapticonoids::VibratorThread","Vibrate!");
				int vibrate_idx = 0;
				if(id >= 0 && id < VIBRATION_LENGTH.length){
					vibrate_idx = id;
				}
				vibra.vibrate(VIBRATION_LENGTH[vibrate_idx]);
			}
		});
	}
	
}
