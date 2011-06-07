#ifndef HIGHSCOREWIDGET_H
#define HIGHSCOREWIDGET_H

#include <MultiWidgets/TextBox.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

struct Highscore {
	std::string name;
	int seconds;
	bool operator<(const Highscore& other) const
    {
        return (seconds <= other.seconds);
    }
};

class HighscoreWidget : public MultiWidgets::Widget {
	
	private:
		std::vector<Highscore> highscores;
		std::vector<MultiWidgets::TextBox*> scoreWidgets;
		
	public:
		HighscoreWidget();
		~HighscoreWidget();
		
		void insertScore(std::string name, int seconds);
		void displayScores();
		void hideScores();
	
};

#endif
