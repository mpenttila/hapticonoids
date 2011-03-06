package fi.hapticonoids;

import java.util.Set;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Vibrator;
import android.widget.Toast;

public class Hapticonoids extends Activity {
	
	private static final int REQUEST_ENABLE_BT = 3;
	Vibrator vibra; 
	BluetoothAdapter mBluetoothAdapter;	
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        this.mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        //this.enableBT();
        
        this.vibra = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);        
    	VibratorThread vt = new VibratorThread(this.vibra);
    	vt.start();    	
    	MessageEater[] eaterarray = {vt};
    	BluetoothClient bs = new BluetoothClient(eaterarray);
    	bs.start();
    }
    
    @Override
    public void onStart() {
        super.onStart();        
        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        Toast.makeText(this, "Trying to enable BT", Toast.LENGTH_LONG).show();
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            Toast.makeText(this, "Enabled BT", Toast.LENGTH_LONG).show();
        }
        Toast.makeText(this, "BT Allready on!", Toast.LENGTH_LONG).show();
    }

}