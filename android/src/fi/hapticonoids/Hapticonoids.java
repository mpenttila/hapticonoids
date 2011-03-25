package fi.hapticonoids;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.widget.TextView;

public class Hapticonoids extends Activity {
	
	public TextView infoField;
	public TextView connectedField;
	
	Vibrator vibra; 
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        this.infoField = (TextView) findViewById(R.id.infoField);
        this.connectedField = (TextView) findViewById(R.id.connectedField);
        this.infoField.setText("Starting app...");
        this.connectedField.setText("");
        
        this.vibra = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);        
    	VibratorThread vt = new VibratorThread(this.vibra, this);
    	vt.start();    	
    	MessageEater[] eaterarray = {vt};
    	BluetoothClient btclient = new BluetoothClient(eaterarray, this);
    	btclient.start();
    }
    
    @Override
    public void onStart() {
        super.onStart();
    }
    
    public void setInfoText(final String text){
    	this.runOnUiThread(new Runnable(){
			public void run() {
				Hapticonoids.this.infoField.setText(text);
			}
    	});
    }
    
    public void setConnectedText(final String text){
    	this.runOnUiThread(new Runnable(){
			public void run() {
				Hapticonoids.this.connectedField.setText(text);
			}
    	});
    }
    
}