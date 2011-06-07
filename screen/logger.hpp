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
		string player1Name;
		string player2Name;
		string gameMode;
		time_t startTime;
		struct tm * localStartTime;
		ofstream logstream;
	
	public:
		Logger();
		~Logger();
		
		void startGame(string mode, string _player1Name, string _player2Name);
		void logGoal(int player);
		void endGame(int winner, int player1Goals, int player2Goals);
	
};


#endif
