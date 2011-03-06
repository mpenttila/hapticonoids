package fi.hapticonoids;

import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;

public final class VibratorThread extends Thread implements MessageEater {

	Vibrator vibra;
	Handler msgQueue = null;
	private static int type = 1;
	
	public VibratorThread(Vibrator vibra) {
		this.vibra = vibra;
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
		return VibratorThread.type;
	}
	
	public synchronized void stopListener() {
		this.msgQueue.post(new Runnable() {
			public void run() {
				Looper.myLooper().quit();				
			}
		});
	}
	
	public synchronized void doTask(int id) {
		this.msgQueue.post(new Runnable() {
			public void run() {
				// TODO Auto-generated method stub
				vibra.vibrate(1000);
				System.out.println("Vibrate!");
			}
		});
	}
	
}
