package fi.hapticonoids;

import android.os.Handler;

public class BluetoothClient extends Thread {

	MessageEater[] eaterarray;
	
	public BluetoothClient(MessageEater[] eaterarray) {
		this.eaterarray = eaterarray;
	}
	
	public void run() {
		for (int i = 0; i < 2; i++) {
			try {
				sleep(5000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("Exception while sleeping.");
			}
			eaterarray[0].doTask(1);
		}
		eaterarray[0].stopListener();
	}
	
}
