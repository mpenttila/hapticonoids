package fi.hapticonoids;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass.Device;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.app.Activity;
import android.os.Looper;
import android.widget.ArrayAdapter;
import android.widget.Toast;

public class BluetoothClient extends Thread {

	private static final int REQUEST_ENABLE_BT = 3;
	private BluetoothAdapter mBluetoothAdapter;
	private MessageEater[] eaterarray;
	private Activity activity;
	private BluetoothSocket game = null;
	private InputStream btIn = null;
	private OutputStream btOut = null;	
	
	
	public BluetoothClient(MessageEater[] eaterarray, Activity activity) {
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
        
        // Get paired devices (We don't currently pair automatically)        
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();        	
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
            	if (device.getName().equals("game")) {
            		try {
            			//UUID tmp = UUID.fromString("1998ea20-cfca-4619-837d-b36b04fde3d5");            			
            			UUID tmp = UUID.fromString("20ea9819-1946-cacf-6bb3-7d83d5e3fd04");
            			this.game = device.createRfcommSocketToServiceRecord(tmp);
            			this.game.connect();
                        this.btIn = this.game.getInputStream();
                        this.btOut = this.game.getOutputStream();            			
            		} catch (IOException e) {
            			System.out.println("Connection failed!" + e.getMessage());
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
        
	}
	
	public void run() {
		Looper.prepare();
		byte[] buffer = new byte[1024];
		int bytes;
		System.out.println("Running the reciver loop!");
		if (this.game == null) {
			System.out.println("Connection failed. Sorry. Game not connected :(");
			this.activity.finish();
			return;
		}
		while (true) {	
			try {
				bytes = this.btIn.read(buffer);
				System.out.println(new String(buffer));
				eaterarray[0].doTask(1);
			} catch (IOException e) {
				System.out.println("Reciver loop ended!" + e.getMessage());
				this.activity.finish();
				break; 
			}
		}		
		eaterarray[0].stopListener();
		Looper.loop();		
	}
	
}
