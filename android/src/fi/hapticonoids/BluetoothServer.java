package fi.hapticonoids;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;

public class BluetoothServer {

	private static final int REQUEST_ENABLE_BT = 3;
	private BluetoothAdapter btAdapter;
	private Activity activity = null;
	private BluetoothServerSocket server = null;
	
	BluetoothServer(Activity activity) {
		this.activity = activity;
		this.btAdapter = BluetoothAdapter.getDefaultAdapter();
        if (!this.btAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            this.activity.startActivityForResult(enableIntent, REQUEST_ENABLE_BT);    
        }
        try {
			this.server = this.btAdapter.listenUsingRfcommWithServiceRecord("A_Hapticonoids", 
							UUID.fromString("20ea9819-1946-cacf-6bb3-7d83d5e3fd04"));
		} catch (IOException e) {
			System.out.print("Error creating server socker!");
		}
	}
	
	public void listen() {
		BluetoothSocket socket = null;
        // Keep listening until exception occurs or a socket is returned
        while (true) {
            try {
            	System.out.println("Waiting to accept connection!");
                socket = server.accept();
                System.out.println("Connection accepted!");
            } catch (IOException e) {
                break;
            }
            // If a connection was accepted
            if (socket != null) {
                // Do work to manage the connection (in a separate thread)
                //manageConnectedSocket(socket);
                try {
					socket.close();
					server.close();
				} catch (IOException e) {
					System.out.print("Closing socket failed!");
				}
                
                break;
            }
        }

		
	}
	
}
