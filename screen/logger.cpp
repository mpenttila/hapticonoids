#include "logger.hpp"

Logger::Logger(){
	time(&startTime);
	localStartTime = localtime(&startTime);
	char date[50];
	strftime(date, sizeof(date), "%Y%m%d_%H%M", localStartTime);
	const string logfilename(string(LOG_FILE) + string(date) + ".log");
	logstream.open(logfilename.c_str());
	if(logstream.is_open()){
		logstream << "Hapticonoids Air Hockey log file created on " << string(asctime (localStartTime)) << endl;
	}
}

Logger::~Logger(){
}

void Logger::startGame(string mode, string _player1Name, string _player2Name){
	gameMode = mode;
	player1Name = _player1Name;
	player2Name = _player2Name;
	time(&startTime);
	localStartTime = localtime(&startTime);
	printf ( "Logger created at: %s", asctime (localStartTime) );
}

void Logger::logGoal(int player){
}

void Logger::endGame(int winner, int player1Goals, int player2Goals){
}
