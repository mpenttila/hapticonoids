#ifndef HIGHSCOREWIDGET_H
#define HIGHSCOREWIDGET_H

#include <MultiWidgets/TextBox.hpp>
#include <MultiWidgets/ImageWidget.hpp>
#include <Fluffy/StyleSheet.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

struct Highscore {
	std::string name;
	long seconds;
	bool operator<(const Highscore& other) const
    {
        return (seconds <= other.seconds);
    }
};

class HighscoreWidget : public MultiWidgets::ImageWidget {
	
	private:
		std::vector<Highscore> highscores;
		std::vector<MultiWidgets::TextBox*> scoreWidgets;
		bool widgetsInitialized;
		Fluffy::StyleSheet _style;
		
	public:
		HighscoreWidget(MultiWidgets::Widget * parent = 0);
		~HighscoreWidget();
		
		void insertScore(std::string name, long seconds);
		void displayScores();
		void hideScores();
	
		void setStyle(const Fluffy::StyleSheet & style);
};

#endif
