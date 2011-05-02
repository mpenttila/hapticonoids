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

/**
 * Main class of hapticonoids Android haptic feedback application. It is run when
 * application is started and it shows the user interface and connects to the game
 * through bluetooth.
 *  
 * @author Veli-Pekka Kestilä, Markus Penttilä
 *
 */
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

        // Fetch the vibrator resource from the Android system. 
        // After acquiring the resource create new vibrator thread to consume
        // vibration events and start it.
        this.vibra = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);        
    	VibratorThread vt = new VibratorThread(this.vibra, this);
    	vt.start();
    	
    	// Create sound thread to consume and process sound playing events.
    	// And start it.
    	SoundThread st = new SoundThread(this);
    	st.start();
    	
    	// Create array containing all message eaters. (sound and vibrations)
    	MessageEater[] eaterarray = {vt, st};
    	
    	// Initialize Bluetooth Client with message eaters.
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
    
    /**
     * Displays given text on the UI information field.
     * @param text
     */
    public void setInfoText(final String text){
    	this.runOnUiThread(new Runnable(){
			public void run() {
				Hapticonoids.this.infoField.setText(text);
			}
    	});
    }
    
    /**
     * Function to connect the game through BT.
     */
    public void onClick(View v) {
    	this.connectButton.setEnabled(false);
    	int player = 1;
    	if(this.radioGroup.getCheckedRadioButtonId() == R.id.radio_p2){
    		player = 2;
    	}	
        this.btclient.connect(this.deviceSpinner.getSelectedItem().toString(), player);
    }
    
    /**
     * Activate connection button.
     */
    public void activateConnectButton(){
    	this.runOnUiThread(new Runnable(){
			public void run() {
				Hapticonoids.this.connectButton.setEnabled(true);
			}
    	});
    }
    
    /**
     * Creates options menu for the program.
     */
    @Override
	public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.main_menu, menu);
        return true;
    }
    
    /**
     * Process events from the options menu.
     */
    @Override
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