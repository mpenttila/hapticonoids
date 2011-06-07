#include "highscorewidget.hpp"
#include <string>

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
			Highscore hs = {name, score};
			highscores.push_back(hs);
			sort(highscores.begin(), highscores.end());
		}
		file.close();
	}
	setInputTransparent(true);
	this->setCSSType("Highscore");
	
	for(int i = 0; i < 10; i++){
		MultiWidgets::TextBox * tb = new MultiWidgets::TextBox(this, 0, MultiWidgets::TextBox::HCENTER);
		scoreWidgets.push_back(tb);
		tb->setInputTransparent(true);
        int scorewidth = tb->totalTextAdvance() + 100;
        tb->setWidth(scorewidth);
        tb->setLocation(size().maximum() * 0.5f - scorewidth/2 - 5, size().minimum() * 0.1f + i * 0.1f);
	}
}

HighscoreWidget::~HighscoreWidget(){
}

void HighscoreWidget::insertScore(string name, int seconds){
	Highscore hs = {name, seconds};
	highscores.push_back(hs);
	sort(highscores.begin(), highscores.end());
	ofstream file;
	file.open("highscores.txt", ios::out | ios::trunc);
	if(file.is_open()){
		for(vector<Highscore>::iterator i = highscores.begin(); i < highscores.end(); i++){
			Highscore hs = *i;
			file << hs.name << ":" << hs.seconds << endl;
		}
	}
	file.close();
}

string convertInt(int number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

void HighscoreWidget::displayScores(){
	vector<Highscore>::iterator i = highscores.begin();
	int count = 0;
	while (i < highscores.end() && count < 10){
		MultiWidgets::TextBox * tb = scoreWidgets[count];
		Highscore hc = *i;
		tb->setText(convertInt(count+1) + ". " + hc.name + " - " + convertInt(hc.seconds) + " s");
		tb->show();
        i++;
        count++;
	}
}

void HighscoreWidget::hideScores(){
	for(vector<MultiWidgets::TextBox*>::iterator i = scoreWidgets.begin(); i < scoreWidgets.end(); i++){
		MultiWidgets::TextBox * tb = *i;
		tb->hide();
	}
}


