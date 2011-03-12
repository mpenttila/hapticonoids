package fi.hapticonoids;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;

public class Hapticonoids extends Activity {
	
	
	Vibrator vibra; 
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        

        
        this.vibra = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);        
    	VibratorThread vt = new VibratorThread(this.vibra);
    	vt.start();    	
    	MessageEater[] eaterarray = {vt};
    	BluetoothClient bs = new BluetoothClient(eaterarray, this);
    	bs.start();
    }
    
    @Override
    public void onStart() {
        super.onStart();
    }

}