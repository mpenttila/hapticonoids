#include "highscorewidget.hpp"
#include <string>

using namespace std;

HighscoreWidget::HighscoreWidget(MultiWidgets::Widget * parent) : 
	MultiWidgets::Widget(parent),
	widgetsInitialized(false)
{
	ifstream file("highscores.txt");
	if(file.is_open()){
		// Read existing highscores
		string line;
		while(file.good()){
			getline(file, line);
			if(line.length() < 3) {
				continue;
			}
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
	setCSSType("Highscore");
	setWidth(400);
	setHeight(300);
	setColor(0,0,0,0);
	
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
	if(!widgetsInitialized){
		for(int i = 0; i < 10; i++){
			MultiWidgets::TextBox * tb = new MultiWidgets::TextBox(this, 0, MultiWidgets::TextBox::HCENTER);
			scoreWidgets.push_back(tb);
			tb->setCSSType("ScoreTextBox");
			tb->setStyle(_style);
			tb->setInputTransparent(true);
			int scorewidth = 300;
			tb->setText(convertInt(i+1) + ". ");
			tb->setWidth(scorewidth);
			tb->setHeight(30);
			tb->setLocation(size().maximum() * 0.5f - scorewidth/2 - 5, 0 + 40 * i);
			tb->show();
		}
		widgetsInitialized = true;
	}
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

void HighscoreWidget::setStyle(const Fluffy::StyleSheet & style){
	MultiWidgets::Widget::setStyle(style);
	_style = style;
}


