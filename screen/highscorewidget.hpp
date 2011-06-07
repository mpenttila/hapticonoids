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
}

class HighscoreWidget : public MultiWidgets::TextBox {
	
	private:
		std::vector<Highscore> highscores;
		
	public:
		HighscoreWidget();
		~HighscoreWidget();
		
		void insertScore(string name, int seconds);
			
	
};

#endif
