package fi.hapticonoids;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.TextView;

public class Hapticonoids extends Activity implements OnClickListener{
	
	public TextView infoField;
	private RadioGroup radioGroup;
	private BluetoothClient btclient;
	private Button connectButton;
	private Spinner deviceSpinner;
	
	Vibrator vibra; 
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        this.infoField = (TextView) findViewById(R.id.infoField);
        this.infoField.setText("");
        this.connectButton = (Button) findViewById(R.id.connectButton);
        this.connectButton.setOnClickListener(this);
        
        this.radioGroup = (RadioGroup) findViewById(R.id.radioGroup1);
                
        this.vibra = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);        
    	VibratorThread vt = new VibratorThread(this.vibra, this);
    	vt.start();
    	SoundThread st = new SoundThread(this);
    	st.start();
    	MessageEater[] eaterarray = {vt, st};
    	this.btclient = new BluetoothClient(eaterarray, this);
    	
    	this.deviceSpinner = (Spinner) findViewById(R.id.spinner1);
    	ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, btclient.getPairedDevices());
    	adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
    	this.deviceSpinner.setAdapter(adapter);
    	
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
    
    public void onClick(View v) {
    	this.connectButton.setEnabled(false);
    	int player = 1;
    	if(this.radioGroup.getCheckedRadioButtonId() == R.id.radio_p2){
    		player = 2;
    	}	
        this.btclient.connect(this.deviceSpinner.getSelectedItem().toString(), player);
    }
    
    public void activateConnectButton(){
    	this.runOnUiThread(new Runnable(){
			public void run() {
				Hapticonoids.this.connectButton.setEnabled(true);
			}
    	});
    }
    
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main_menu, menu);
        return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
        case R.id.exit:
            this.finish();
            return true;
        default:
            return super.onOptionsItemSelected(item);
        }
    }

}