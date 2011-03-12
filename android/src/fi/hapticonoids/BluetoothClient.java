package fi.hapticonoids;

import java.util.Set;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass.Device;
import android.bluetooth.BluetoothDevice;
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
	BluetoothAdapter mBluetoothAdapter;
	MessageEater[] eaterarray;
	Activity activity;
	ArrayAdapter<String> btDeviceArray;
	
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
        //this.enableBT();

        
	}
	
	public void run() {
		Looper.prepare();
        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        Toast.makeText(this.activity, "Trying to enable BT", Toast.LENGTH_LONG).show();
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            this.activity.startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            Toast.makeText(this.activity, "Enabled BT", Toast.LENGTH_LONG).show();
        }
        else
        	Toast.makeText(this.activity, "BT Allready on!", Toast.LENGTH_LONG).show();
        
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        // If there are paired devices
        if (pairedDevices.size() > 0) {
        	// Loop through paired devices
        	for (BluetoothDevice device : pairedDevices) {
        		// Add the name and address to an array adapter to show in a ListView
        		this.btDeviceArray.add(device.getName() + "\n" + device.getAddress());
        		//Toast.makeText(this.activity, device.getName() + "\n" + device.getAddress(), Toast.LENGTH_LONG).show();
        	}
        }
        
        // Create a BroadcastReceiver for ACTION_FOUND
        System.out.println("Recive devices");
        final BroadcastReceiver mReceiver = new BroadcastReceiver() {        	
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                // When discovery finds a device
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    // Get the BluetoothDevice object from the Intent
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    // Add the name and address to an array adapter to show in a ListView
                    //this.btDeviceArray.add(device.getName() + "\n" + device.getAddress());
                    System.out.println(device.getName() + "\n" + device.getAddress());
                }
            }
        };
        // Register the BroadcastReceiver
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.activity.registerReceiver(mReceiver, filter); // Don't forget to unregister during onDestroy        
        
        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
        this.activity.startActivity(discoverableIntent);        
        
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
		Looper.loop();		
	}
	
}
