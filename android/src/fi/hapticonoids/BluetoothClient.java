package fi.hapticonoids;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothClass.Device;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.app.Activity;
import android.os.Looper;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.Toast;

public class BluetoothClient extends Thread {

	private static final int REQUEST_ENABLE_BT = 3;
	private BluetoothAdapter mBluetoothAdapter;
	private MessageEater[] eaterarray;
	private Hapticonoids activity;
	private BluetoothSocket game = null;
	private InputStream btIn = null;
	private OutputStream btOut = null;	
	
	
	public BluetoothClient(MessageEater[] eaterarray, Hapticonoids activity) {
		this.activity = activity;
		this.eaterarray = eaterarray;
        this.mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this.activity, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            this.activity.finish();
            return;
        }
        // Enable BT
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            this.activity.startActivityForResult(enableIntent, REQUEST_ENABLE_BT);    
        }
        
        
	}
	
	public void run() {
		// Get paired devices (We don't currently pair automatically)
        this.activity.setInfoText("Searching paired devices");
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();        	
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
            	Log.i("Hapticonoids::BluetoothClient", "Paired device: "+device.getName());
            	int btclass = device.getBluetoothClass().getMajorDeviceClass();
            	if (btclass == BluetoothClass.Device.COMPUTER_UNCATEGORIZED ||
            			btclass == BluetoothClass.Device.COMPUTER_DESKTOP ||
            			btclass == BluetoothClass.Device.COMPUTER_LAPTOP ||
            			btclass == BluetoothClass.Device.COMPUTER_SERVER) {
            		try {
            			this.activity.setInfoText("Connecting to "+device.getName());
            			//UUID tmp = UUID.fromString("1998ea20-cfca-4619-837d-b36b04fde3d5");            			
            			UUID tmp = UUID.fromString("20ea9819-1946-cacf-6bb3-7d83d5e3fd04");
            			Log.i("Hapticonoids::BluetoothClient","UUID: "+tmp.toString());
            			this.game = device.createRfcommSocketToServiceRecord(tmp);
            			this.game.connect();
                        this.btIn = this.game.getInputStream();
                        this.btOut = this.game.getOutputStream();
                        this.activity.setInfoText("Game found, listening for events.");
                        this.activity.setConnectedText("Connected to "+device.getName());
            		} catch (IOException e) {
            			Log.i("Hapticonoids::BluetoothClient","Connection failed: " + e.getMessage());
            			this.activity.setInfoText("Connection failed!");
            			if (this.game != null) {
	            			try {
								this.game.close();
							} catch (IOException e1) {
								// TODO Auto-generated catch block
								e1.printStackTrace();
							}
	            			this.game = null;
            			}
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
				Log.i("Hapticonoids::BluetoothClient","Client not found, exiting.");
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
		Log.i("Hapticonoids::BluetoothClient","Game found, running the receiver loop!");
		while (true) {	
			try {
				bytes = this.btIn.read(buffer);
				// Construct string
				StringBuilder message = new StringBuilder();
				for(int i = 0; i < bytes; i++){
					message.append((char)buffer[i]);
				}
				Log.i("Hapticonoids::BluetoothClient",message.toString());
				String parts[] = message.toString().split(":");
				eaterarray[0].doTask(Integer.parseInt(parts[1]));
			} catch (IOException e) {
				Log.i("Hapticonoids::BluetoothClient","Receiver loop ended: " + e.getMessage());
				this.activity.finish();
				break; 
			}
		}		
		eaterarray[0].stopListener();
		Looper.loop();		
	}
	
}
