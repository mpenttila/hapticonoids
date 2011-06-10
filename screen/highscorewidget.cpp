#include "highscorewidget.hpp"
#include <string>

using namespace std;

HighscoreWidget::HighscoreWidget(MultiWidgets::Widget * parent) : 
	MultiWidgets::ImageWidget(parent),
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
			long score = atol(line.substr(found + 1).c_str());
			Highscore hs = {name, score};
			highscores.push_back(hs);
			sort(highscores.begin(), highscores.end());
		}
		file.close();
	}
	load(std::string("background_highscores.png"));
	setInputTransparent(true);
	//setCSSType("Highscore");
	setWidth(800);
	setHeight(400);
	
}

HighscoreWidget::~HighscoreWidget(){
}

void HighscoreWidget::insertScore(string name, long seconds){
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

string convertInt(long number)
{
   stringstream ss;
   ss << number;
   return ss.str();
}

void HighscoreWidget::displayScores(){
	if(!widgetsInitialized){
		MultiWidgets::TextBox * title = new MultiWidgets::TextBox(this, "Fastest winners", MultiWidgets::TextBox::HCENTER);
		title->setCSSType("HighscoreTitle");
		title->setStyle(_style);
		title->setInputTransparent(true);
		title->setWidth(width() - 20);
		title->setLocation(width() * 0.5f - title->width()/2, 20);
		for(int i = 0; i < 10; i++){
			MultiWidgets::TextBox * tb = new MultiWidgets::TextBox(this, 0, MultiWidgets::TextBox::HCENTER);
			scoreWidgets.push_back(tb);
			tb->setCSSType("ScoreTextBox");
			tb->setStyle(_style);
			tb->setInputTransparent(true);
			tb->setText(convertInt(i+1) + ". ");
			tb->setWidth(width() - 20);
			tb->setHeight(30);
			tb->setLocation(size().maximum() * 0.5f - tb->width()/2, 60 + 32 * i);
			tb->show();
		}
		widgetsInitialized = true;
	}
	vector<Highscore>::iterator i = highscores.begin();
	int count = 0;
	while (i < highscores.end() && count < 10){
		MultiWidgets::TextBox * tb = scoreWidgets[count];
		Highscore hc = *i;
		tb->setText(convertInt(count+1) + ". " + hc.name + " - " + convertInt(hc.seconds) + " ms");
        i++;
        count++;
	}
	for(int i = 0; i < 10; i++){
		MultiWidgets::TextBox * tb = scoreWidgets[i];
		tb->show();
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


