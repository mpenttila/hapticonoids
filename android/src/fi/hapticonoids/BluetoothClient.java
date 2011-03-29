package fi.hapticonoids;

import java.util.ArrayList;
import java.util.Set;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.widget.Toast;

public class BluetoothClient{

	private static final int REQUEST_ENABLE_BT = 3;
	private BluetoothAdapter mBluetoothAdapter;
	private MessageEater[] eaterarray;
	private Hapticonoids activity;
	private BluetoothSocket game = null;
	
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
	
	public String[] getPairedDevices(){
		Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
		ArrayList<String> computers = new ArrayList<String>();
		for(BluetoothDevice device : pairedDevices){
			int btclass = device.getBluetoothClass().getMajorDeviceClass();
        	if (btclass == BluetoothClass.Device.COMPUTER_UNCATEGORIZED ||
        			btclass == BluetoothClass.Device.COMPUTER_DESKTOP ||
        			btclass == BluetoothClass.Device.COMPUTER_LAPTOP ||
        			btclass == BluetoothClass.Device.COMPUTER_SERVER) {
        		computers.add(device.getName());
        	}
		}
		return computers.toArray(new String[computers.size()]);
	}

	public void connect(String device, int player){
		BluetoothThread btThread = new BluetoothThread(device, activity, game, mBluetoothAdapter, eaterarray, player);
		new Thread(btThread).start();
	}
	
	
}
