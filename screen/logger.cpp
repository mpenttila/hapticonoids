#include <algorithm>
#include "logger.hpp"

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

wstring strToWstr(const string & str ){
	wstring temp(str.length(), L' ');
	copy(str.begin(), str.end(), temp.begin());
	return temp;
}

void Logger::startGame(string mode, wstring _player1Name, wstring _player2Name){
	gameNumber++;
	wstring wideMode = strToWstr(mode);
	player1Name = _player1Name;
	player2Name = _player2Name;
	time(&startTime);
	localStartTime = localtime(&startTime);
	ptime t(microsec_clock::local_time());
	startMicroTime = t;
	wstring timeString = strToWstr(to_simple_string(startMicroTime));
	logstream << timeString << endl;
	logstream << "ROUND " << gameNumber << ": New game started in mode: " << wideMode << endl;
	logstream << "ROUND " << gameNumber << ": Player 1: " << _player1Name << ", Player 2: " << _player2Name << endl;
}

void Logger::logGoal(int player){
	ptime goalTime(microsec_clock::local_time());
	wstring playerName;
	if(player == 1) playerName = player1Name;
	else if(player == 2) playerName = player2Name;
	else return;
	time_duration difference = goalTime - startMicroTime;
	logstream << "ROUND " << gameNumber << ": [" << difference.total_milliseconds() << " ms]: Player " << player << " (" << playerName << ") scored a goal" << endl;
}

// Returns seconds passed for highscores
long Logger::endGame(int winner, int player1Goals, int player2Goals){
	ptime endTime(microsec_clock::local_time());
	wstring playerName;
	if(winner == 1) playerName = player1Name;
	else if(winner == 2) playerName = player2Name;
	time_duration difference = endTime - startMicroTime;
	logstream << "ROUND " << gameNumber << ": [" << difference.total_milliseconds() << " ms]: Winner is player " << winner << " (" << playerName << ")" << endl;
	logstream << "ROUND " << gameNumber << ": Final score: Player 1 (" << player1Name << "): " << player1Goals << " - " << "Player 2 (" << player2Name << "): " << player2Goals << endl;
	logstream << endl;
	return difference.total_milliseconds();
}
