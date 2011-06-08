#include <algorithm>
#include "logger.hpp"
#include "utf8.h"

Logger::Logger() : gameNumber(0) {
	time(&startTime);
	localStartTime = localtime(&startTime);
	char date[50];
	strftime(date, sizeof(date), "%Y%m%d_%H%M", localStartTime);
	const string logfilename(string(LOG_FILE) + string(date) + ".log");
	logstream.open(logfilename.c_str());
	if(logstream.is_open()){
		logstream << "Hapticonoids Air Hockey log file created on " << asctime (localStartTime) << endl;
	}
}

Logger::~Logger(){
	logstream.close();
}

void Logger::startGame(string mode, wstring _player1Name, wstring _player2Name){
	wstring wideMode(mode.length(), L' ');
	copy(mode.begin(), mode.end(), wideMode.begin());
	player1Name = _player1Name;
	player2Name = _player2Name;
	time(&startTime);
	localStartTime = localtime(&startTime);
	gameNumber++;
	logstream << asctime(localStartTime) << "ROUND " << gameNumber << ": New game started in mode: " << wideMode << endl;
	logstream << "ROUND " << gameNumber << ": Player 1: " << _player1Name << ", Player 2: " << _player2Name << endl;
}

void Logger::logGoal(int player){
	time_t goalTime;
	time(&goalTime);
	wstring playerName;
	if(player == 1) playerName = player1Name;
	else if(player == 2) playerName = player2Name;
	else return;
	time_t difference = goalTime - startTime;
	logstream << "ROUND " << gameNumber << ": [" << difference << " s]: Player " << player << " (" << playerName << ") scored a goal" << endl;
}

// Returns seconds passed for highscores
long Logger::endGame(int winner, int player1Goals, int player2Goals){
	time_t endTime;
	time(&endTime);
	wstring playerName;
	if(winner == 1) playerName = player1Name;
	else if(winner == 2) playerName = player2Name;
	time_t difference = endTime - startTime;
	logstream << "ROUND " << gameNumber << ": [" << difference << " s]: Winner is player " << winner << " (" << playerName << ")" << endl;
	logstream << "ROUND " << gameNumber << ": Final score: Player 1 (" << player1Name << "): " << player1Goals << " - " << "Player 2 (" << player2Name << "): " << player2Goals << endl;
	logstream << endl;
	return (long)difference;
}
