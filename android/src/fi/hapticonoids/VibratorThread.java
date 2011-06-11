package fi.hapticonoids;

import java.util.ArrayList;

import android.os.Handler;
import android.os.Looper;
import android.os.Vibrator;
import android.util.Log;

/**
 * Class realizing the thread to process vibration events.
 * @author Veli-Pekka Kestila, Markus Penttila
 *
 */
public final class VibratorThread extends Thread implements MessageEater {

	Vibrator vibra;
	Handler msgQueue = null;
	private static int type = 0;
	private Hapticonoids hapticonoids;
	
	//public static final int[] VIBRATION_LENGTH = {0, 200, 400, 600, 800, 1000};
	
	private ArrayList<VibrationPattern> patterns;
	
	// first value off, second value on, third off etc.
	private static final long[][] VIBRATION_PATTERNS = {{0}, {0,200}, {0,100,50,100}, {0,400,200,400}};
	
	/**
	 * Constructor accepting the vibration resource and the main activity.
	 * @param vibra
	 * @param hapticonoids
	 */
	public VibratorThread(Vibrator vibra, Hapticonoids hapticonoids) {
		this.vibra = vibra;
		this.hapticonoids = hapticonoids;
		this.patterns = new ArrayList<VibrationPattern>();
		// Create patterns to list
		VibrationPattern temp = new VibrationPattern();
		temp.addToSequence(false, 0);
		patterns.add(temp);
		
		temp = new VibrationPattern();
		temp.addToSequence(true, 200);
		patterns.add(temp);
		
		temp = new VibrationPattern();
		temp.addToSequence(true, 100);
		temp.addToSequence(false, 50);
		temp.addToSequence(true, 100);
		patterns.add(temp);
		
		temp = new VibrationPattern();
		temp.addToSequence(true, 400);
		temp.addToSequence(false, 200);
		temp.addToSequence(true, 400);
		patterns.add(temp);
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
				//hapticonoids.setInfoText("Vibrating!");
				Log.i("Hapticonoids::VibratorThread","Vibrate!");
				int vibrate_idx = 0;
				if(id >= 0 && id < VIBRATION_PATTERNS.length){
					vibrate_idx = id;
				}
				vibra.vibrate(VIBRATION_PATTERNS[vibrate_idx], -1);
				/*for(VibrationType type : pattern.getSequence()){
					try{
						if(type.vibrate){
							vibra.vibrate(type.length);
							Thread.sleep(type.length);
						}
						else{
							// Pause in sequence
							Thread.sleep(type.length);
						}
					}
					catch(InterruptedException ie){
						// Do nothing
					}
				}
				*/
			}
		});
	}
	
	private class VibrationType{
		public boolean vibrate;
		public int length;
		
		public VibrationType(boolean vibrate, int length){
			this.vibrate = vibrate;
			this.length = length;
		}
	}
	
	private class VibrationPattern{
		
		private ArrayList<VibrationType> sequence;
		
		public VibrationPattern(){
			this.sequence = new ArrayList<VibrationType>();
		}
		
		public void addToSequence(boolean vibrate, int length){
			this.sequence.add(new VibrationType(vibrate, length));
		}
		
		public ArrayList<VibrationType> getSequence(){
			return this.sequence;
		}
	}
	
}
