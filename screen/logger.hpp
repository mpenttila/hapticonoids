#ifndef HAPTILOGGER_H
#define HAPTILOGGER_H
 
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

#define LOG_FILE "logs/hapticonoids_game_"

using namespace std;

class Logger {
	private:
		wstring player1Name;
		wstring player2Name;
		time_t startTime;
		struct tm * localStartTime;
		wofstream logstream;
		int gameNumber;
	
	public:
		Logger();
		~Logger();
		
		void startGame(string mode, wstring _player1Name, wstring _player2Name);
		void logGoal(int player);
		long endGame(int winner, int player1Goals, int player2Goals);
	
};


#endif
