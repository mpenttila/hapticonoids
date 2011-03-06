package fi.hapticonoids;

public interface MessageEater {
	int getType();
	void stopListener();
	void doTask(int id);
}
