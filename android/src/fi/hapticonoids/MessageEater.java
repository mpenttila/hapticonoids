package fi.hapticonoids;

/**
 * Interface both Sound and Vibration event processor present for the 
 * Bluetooth client thread.
 * @author Veli-Pekka Kestilä
 *
 */
public interface MessageEater {
	int getType();
	void stopListener();
	void doTask(int id);
}
