#include "logger.hpp"

Logger::Logger() : gameNumber(0) {
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
	logstream.close();
}

void Logger::startGame(string mode, string _player1Name, string _player2Name){
	gameMode = mode;
	player1Name = _player1Name;
	player2Name = _player2Name;
	time(&startTime);
	localStartTime = localtime(&startTime);
	gameNumber++;
	logstream << asctime(localStartTime) << "ROUND " << gameNumber << ": New game started in mode: " << mode << endl;
	logstream << "Player 1: " << _player1Name << ", Player 2: " << _player2Name << endl;
}

void Logger::logGoal(int player){
	time_t goalTime;
	time(&goalTime);
	string playerName;
	if(player == 1) playerName = player1Name;
	else if(player == 2) playerName = player2Name;
	else return;
	time_t difference = goalTime - startTime;
	logstream << "[" << difference << " s]: Player " << player << " (" << playerName << ") scored a goal" << endl;
}

void Logger::endGame(int winner, int player1Goals, int player2Goals){
}
