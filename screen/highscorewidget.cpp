#include "highscorewidget.hpp"

using namespace std;

HighscoreWidget::HighscoreWidget(){
	ifstream file("highscore.txt");
	if(file.is_open()){
		// Read existing highscores
		string line;
		while(file.good()){
			getline(file, line);
			size_t found = line.find_last_of(":");
			string name(line.substr(0, found));
			int score = atoi(line.substr(found + 1).c_str());
			highscores.
		}
		file.close();
	}
}

HighscoreWidget::~HighscoreWidget(){
}

HighscoreWidget::insertScore(string name, int seconds){
}
