package fi.hapticonoids;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Looper;
import android.util.Log;

public class BluetoothThread implements Runnable{
	
	private String connectionPartner;
	private Hapticonoids activity;
	private BluetoothSocket game = null;
	private BluetoothAdapter mBluetoothAdapter;
	private InputStream btIn = null;
	private OutputStream btOut = null;
	private MessageEater[] eaterarray;
	private char player;
	
	public BluetoothThread(String connectionPartner, Hapticonoids activity,
			BluetoothSocket game, BluetoothAdapter mBluetoothAdapter,
			MessageEater[] eaterarray, int player) {
		super();
		this.connectionPartner = connectionPartner;
		this.activity = activity;
		this.game = game;
		this.mBluetoothAdapter = mBluetoothAdapter;
		this.eaterarray = eaterarray;
		this.player = Integer.toString(player).charAt(0);
	}

	public void run() {
		if(this.connectionPartner == null) return;
		this.activity.setInfoText("Connecting to "+this.connectionPartner+"...");
		// Get paired devices (We don't currently pair automatically)
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();        	
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
            	Log.i("Hapticonoids::BluetoothThread", "Paired device: "+device.getName());
            	if (device.getName().equals(this.connectionPartner)){
            		try {
            			this.activity.setInfoText("Connecting to "+device.getName());
            			//UUID tmp = UUID.fromString("1998ea20-cfca-4619-837d-b36b04fde3d5");            			
            			UUID tmp = UUID.fromString("20ea9819-1946-cacf-6bb3-7d83d5e3fd04");
            			Log.i("Hapticonoids::BluetoothThread","UUID: "+tmp.toString());
            			this.game = device.createRfcommSocketToServiceRecord(tmp);
            			this.game.connect();
                        this.btIn = this.game.getInputStream();
                        this.btOut = this.game.getOutputStream();
                        this.activity.setInfoText("Game found, listening for events.");
            		} catch (IOException e) {
            			Log.i("Hapticonoids::BluetoothThread","Connection failed: " + e.getMessage());
            			this.activity.setInfoText("Connection failed!");
            			this.activity.activateConnectButton();
            			return;
            		}
            	}
            }
        }
		
		
		Looper.prepare();
		byte[] buffer = new byte[1024];
		int bytes;
		int timeout = 20;
		while(this.game == null){
			if(timeout == 0){
				Log.i("Hapticonoids::BluetoothThread","Client not found, exiting.");
				this.activity.finish();
				return;
			}
			try{
				Thread.sleep(500);
			}
			catch(InterruptedException ie){
				break;
			}
			--timeout;
		}
		Log.i("Hapticonoids::BluetoothThread","Game found, starting data transfer.");
		
		try {
			this.btOut.write(this.player);
		} catch (IOException e1) {
			Log.e("Hapticonoids::BluetoothThread", "Could not write to bt stream: "+e1.getMessage());
		}
		
		Log.i("Hapticonoids::BluetoothThread","Running the receiver loop.");
		while (true) {	
			try {
				bytes = this.btIn.read(buffer);
				// Construct string
				StringBuilder message = new StringBuilder();
				for(int i = 0; i < bytes; i++){
					message.append((char)buffer[i]);
				}
				Log.i("Hapticonoids::BluetoothThread",message.toString());
				String parts[] = message.toString().split(":");
				eaterarray[Integer.parseInt(parts[0])].doTask(Integer.parseInt(parts[1]));
			} catch (IOException e) {
				Log.i("Hapticonoids::BluetoothThread","Receiver loop ended: " + e.getMessage());
				this.activity.finish();
				break; 
			}
		}		
		eaterarray[0].stopListener();
		Looper.loop();		
	}
	
}
