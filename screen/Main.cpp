/* COPYRIGHT
 *
 * This file is part of HelloImages.
 *
 * Copyright: MultiTouch Oy, Finland, http://multitou.ch
 *
 * All rights reserved, 2007-2009
 *
 * You may use this file only for purposes for which you have a
 * specific, written permission from MultiTouch Oy.
 *
 * See file "HelloImages.hpp" for authors and more details.
 *
 */

#include <airhockeywidget.hpp>
#include <contactlistener.hpp>

#include <MultiWidgets/ImageWidget.hpp>
#include <MultiWidgets/TextBox.hpp>
#include <MultiWidgets/Keyboard.hpp>
#include <MultiWidgets/TextEdit.hpp>
#include <MultiWidgets/SimpleSDLApplication.hpp>
#include <MultiWidgets/StayInsideParentOperator.hpp>
#include <MultiWidgets/Animators.hpp>

#include <Radiant/Directory.hpp>

#define MALLET1_VERTICAL_FRACTION 0.15f
#define MALLET2_VERTICAL_FRACTION 0.85f


class Hapticonoids : public MultiWidgets::SimpleSDLApplication
{
	typedef MultiWidgets::SimpleSDLApplication Parent;
	
	class Receiver : public virtual MultiWidgets::Widget
	{
		public:
		// Normal constructor
		Receiver(MultiWidgets::Widget * parent)
			: Widget(parent)
		{
			// Make this widget ignore all input
			setInputFlags(0);
		}
		virtual ~Receiver() {}
		
		virtual void processMessage(const char * id, Radiant::BinaryData & data)
		{
			if(strcmp(id, "jump") == 0) {
			}
		}
	};

private:
	AirHockeyWidget * gw;
	MultiWidgets::ImageWidget * mallet1, * mallet2, * puck;
	MultiWidgets::TextBox * p1, * p2;
	MultiWidgets::TextBox * b0, * b1, * b2, * b3;
	MultiWidgets::Keyboard * kb1, * kb2;
	MultiWidgets::TextEdit * text1, * text2;
	
public:
	Hapticonoids() : Parent()
	{
	}
	
	void initializeWidgets()
	{
		gw = new AirHockeyWidget(root(), this);
		gw->setSize(root()->size());
		gw->setStyle(style());
		gw->setDepth(0);
		gw->setInputTransparent(true);
		gw->setColor(0, 0, 0, 0);

		float malletsize = size().minimum() * 0.1f;
		float pucksize = size().minimum() * 0.07f;
		float vertical_middle = size().minimum() * 0.5f;

		float mallet1_horizontal = size().maximum() * MALLET1_VERTICAL_FRACTION;
		float mallet2_horizontal = size().maximum() * MALLET2_VERTICAL_FRACTION;
		float puck_horizontal = size().maximum() * 0.5f;

		// Create image widgets:
		mallet1 = new MultiWidgets::ImageWidget();
		mallet2 = new MultiWidgets::ImageWidget();
		puck = new MultiWidgets::ImageWidget();  

		/* Check that the file is loadable. The actual file loading is
		  done in a background thread, so that the loading does not stall
		  the application. */
		if(mallet1->load(std::string("mallet.png")) && mallet2->load(std::string("mallet.png"))) {

			 // Resize the Widget:
			 mallet1->resizeToFit(Nimble::Vector2(malletsize, malletsize));
			 mallet2->resizeToFit(Nimble::Vector2(malletsize, malletsize));
			 // Set location
			 mallet1->setCenterLocation(Nimble::Vector2(mallet1_horizontal, vertical_middle));
			 mallet2->setCenterLocation(Nimble::Vector2(mallet2_horizontal, vertical_middle));
			 /* Make the image widgets stay inside the parent widget. */
			 mallet1->addOperator(new MultiWidgets::StayInsideParentOperator());
			 mallet2->addOperator(new MultiWidgets::StayInsideParentOperator());
			 gw->addChild(mallet1);
			 gw->addChild(mallet2);
			 gw->mallet1 = mallet1;
			 gw->mallet2 = mallet2;
		}
		else {
			Radiant::error("Could not load image file mallet.png");
			delete mallet1;
			delete mallet2;
		}

		if(puck->load(std::string("puck.png"))) {

			// Resize the Widget:
			puck->resizeToFit(Nimble::Vector2(pucksize, pucksize));
			// Set location
			puck->setCenterLocation(Nimble::Vector2(puck_horizontal, vertical_middle));
			/* Make the image widget stay inside the parent widget. */
			//puck->addOperator(new MultiWidgets::StayInsideParentOperator());
			gw->addChild(puck);
			gw->puck = puck; 
		}
		else {
			Radiant::error("Could not load image file puck.png");
			delete puck;
		}
		
		mallet1->hide();
		mallet2->hide();
		puck->hide();

		gw->scorewidget = new MultiWidgets::TextBox(root(), 0, MultiWidgets::TextBox::HCENTER);
		gw->scorewidget->setStyle(style());
		gw->scorewidget->setText(std::string("0    0"));
		int scorewidth = gw->scorewidget->totalTextAdvance() + 300;
		gw->scorewidget->setWidth(scorewidth);
		gw->scorewidget->setInputTransparent(true);
		gw->scorewidget->setColor(0, 0, 0, 0);
		gw->scorewidget->setLocation(size().maximum() * 0.5f - scorewidth/2 - 5, size().minimum() * 0.1f);
		gw->scorewidget->hide();
   
		p1 = new MultiWidgets::TextBox(root(), 0, MultiWidgets::TextBox::HCENTER);
		p1->setStyle(style());
		p1->setText(std::string("Player 1"));
		p1->setWidth(p1->totalTextAdvance() + 50);
		p1->setInputTransparent(true);
		p1->setColor(0, 0, 0, 0);
		p1->setRotation(Nimble::Math::HALF_PI);
		p1->setLocation(size().maximum() * 0.1f, size().minimum() * 0.5f - p1->totalTextAdvance()/2 - 10);
		p1->hide();

		p2 = new MultiWidgets::TextBox(root(), 0, MultiWidgets::TextBox::HCENTER);
		p2->setStyle(style());
		p2->setText(std::string("Player 2"));
		p2->setWidth(p2->totalTextAdvance() + 50);
		p2->setInputTransparent(true);
		p2->setColor(0, 0, 0, 0);
		p2->setRotation(-1 * Nimble::Math::HALF_PI);
		//p2->setLocation(size().maximum() * 0.9f, size().minimum() * 0.5f - p2->totalTextAdvance()/2 - 10));
		p2->setLocation(size().maximum() * 0.9f, size().minimum() - p2->totalTextAdvance());
		p2->hide();

		HighscoreWidget * highscore = new HighscoreWidget(root());
		highscore->setStyle(style());
		highscore->setLocation(size().maximum() * 0.5f - highscore->width()/2, size().minimum() * 0.3f - highscore->height()/2);
		gw->highscore = highscore;
		highscore->displayScores();
		
		// Start buttons
		b0 = new MultiWidgets::TextBox(root(), "Start without feedback");
		b1 = new MultiWidgets::TextBox(root(), "Start with feedback to both");
		b2 = new MultiWidgets::TextBox(root(), "Start with feedback to P1");
		b3 = new MultiWidgets::TextBox(root(), "Start with feedback to P2");
		
		b0->eventAddListener("interactionbegin", "start0", gw);
		b0->setCSSType("StartButton");
		b0->setStyle(style());
		int buttonWidth = b0->size().maximum();
		b0->setLocation(size().maximum() * 0.5f - 100 - 2*buttonWidth, size().minimum() - 200);
		b0->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b1->eventAddListener("interactionbegin", "start1", gw);
		b1->setCSSType("StartButton");
		b1->setStyle(style());
		b1->setLocation(size().maximum() * 0.5f - 50 - buttonWidth, size().minimum() - 200);
		b1->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b2->eventAddListener("interactionbegin", "start2", gw);
		b2->setCSSType("StartButton");
		b2->setStyle(style());
		b2->setLocation(size().maximum() * 0.5f + 50, size().minimum() - 200);
		b2->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b3->eventAddListener("interactionbegin", "start3", gw);
		b3->setCSSType("StartButton");
		b3->setStyle(style());
		b3->setLocation(size().maximum() * 0.5f + 100 + buttonWidth, size().minimum() - 200);
		b3->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		kb1 = MultiWidgets::Keyboard::create(root(), "fi");
		kb1->setStyle(style());
		kb1->setLocation(300, 80);
		kb1->setRotation(Nimble::Math::HALF_PI);
		kb1->addOperator(new MultiWidgets::StayInsideParentOperator());
		
		kb2 = MultiWidgets::Keyboard::create(root(), "fi");
		kb2->setStyle(style());
		kb2->setLocation(size().maximum()-300, 80);
		kb2->setRotation(-1 * Nimble::Math::HALF_PI);
		kb2->addOperator(new MultiWidgets::StayInsideParentOperator());
		
		//MultiWidgets::TextBox * text1Label = new MultiWidgets::TextBox(
		
		text1 = new MultiWidgets::TextEdit(root(), "Enter player 1 name", MultiWidgets::TextEdit::HCENTER);
		text1->setKeyboard(kb1);
		text1->setColor(Radiant::Color(1.0f, 0.3f, 0.6f, 1.0f)); // Hot pink!
		text1->Widget::setSize(240, 70);
		text1->setRotation(Nimble::Math::HALF_PI);
		text1->setLocation(500, size().minimum() * 0.5f - 120);
		text1->setCSSType("NameInput");
		text1->setStyle(style());
		text1->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		text2 = new MultiWidgets::TextEdit(root(), "Enter player 2 name", MultiWidgets::TextEdit::HCENTER);
		text2->setKeyboard(kb2);
		text2->setColor(Radiant::Color(1.0f, 0.3f, 0.6f, 1.0f)); // Hot pink!
		text2->Widget::setSize(240, 70);
		text2->setRotation(-1 * Nimble::Math::HALF_PI);
		text2->setLocation(size().maximum() - 500, size().minimum() * 0.5f + 120);
		text2->setCSSType("NameInput");
		text2->setStyle(style());
		text2->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		text1->hideKeyboard();
		text1->hide();
		text2->hideKeyboard();
		text2->hide();
		
		gw->b0 = b0;
		gw->b1 = b1;
		gw->b2 = b2;
		gw->b3 = b3;
		gw->text1 = text1;
		gw->text2 = text2;
//		gw->kb1 = kb1;

		gw->initBluetooth();
	}
	
	void initGame(int _feedbackMode){
		b0->hide();
		b1->hide();
		b2->hide();
		b3->hide();
		text1->show();
		kb1->show();
	}
	
};

int main(int argc, char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	Hapticonoids app;

	if(!app.simpleInit(argc, argv))
		return 1;

	app.setStyleFile("style.css");

	MultiWidgets::ImageWidget * background = new MultiWidgets::ImageWidget();
	background->load(std::string("air-hockey.png"));
	background->resizeToFit(app.root()->size());
	background->setDepth(-30);
	background->setInputTransparent(true);
	app.root()->addChild(background);
	
	app.initializeWidgets();

	// Run the application:
	return app.run();
}
