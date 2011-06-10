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

private:
	AirHockeyWidget * gw;
	MultiWidgets::ImageWidget * mallet1, * mallet2, * puck;
	MultiWidgets::TextBox * p1, * p2;
	MultiWidgets::TextBox * b0, * b1, * b2, * b3, * b4;
	MultiWidgets::TextEdit * text1, * text2;
	
public:
	Hapticonoids() : Parent()
	{
	}
	
	void initializeWidgets()
	{
		gw = new AirHockeyWidget(root());
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
   
		p1 = new MultiWidgets::TextBox(root(), 0, MultiWidgets::TextBox::HCENTER | MultiWidgets::TextBox::VCENTER);
		p1->setStyle(style());
		p1->setWidth(500);
		p1->setInputTransparent(true);
		p1->setColor(0, 0, 0, 0);
		p1->setRotation(Nimble::Math::HALF_PI);
		p1->setLocation(size().maximum() * 0.05f + p1->height(), size().minimum() * 0.5f - p1->width()/2);
		p1->hide();

		p2 = new MultiWidgets::TextBox(root(), 0, MultiWidgets::TextBox::HCENTER | MultiWidgets::TextBox::VCENTER);
		p2->setStyle(style());
		p2->setText(std::string("Player 2"));
		p2->setWidth(500);
		p2->setInputTransparent(true);
		p2->setColor(0, 0, 0, 0);
		p2->setRotation(-1 * Nimble::Math::HALF_PI);
		p2->setLocation(size().maximum() * 0.95f - p2->height(), size().minimum() * 0.5f + p2->width()/2);
		p2->hide();

		HighscoreWidget * highscore = new HighscoreWidget(root());
		highscore->setStyle(style());
		highscore->setLocation(size().maximum() * 0.5f - highscore->width()/2, size().minimum() * 0.1f);
		gw->highscore = highscore;
		highscore->displayScores();
		
		MultiWidgets::TextBox * winnerLabel = new MultiWidgets::TextBox(root(), "PlayerPlayerPlayer 1 wins 2-0!", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		winnerLabel->setCSSType("WinnerLabel");
		winnerLabel->setStyle(style());
		winnerLabel->setWidth(winnerLabel->totalTextAdvance() + 300);
		winnerLabel->setLocation(size().maximum() * 0.5f - winnerLabel->width()/2, size().minimum() * 0.6f);
		winnerLabel->setInputTransparent(true);
		winnerLabel->hide();
		
		// Start buttons
		b0 = new MultiWidgets::TextBox(root(), "Start without feedback", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		b1 = new MultiWidgets::TextBox(root(), "Start with feedback to both", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		b2 = new MultiWidgets::TextBox(root(), "Start with feedback to P1", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		b3 = new MultiWidgets::TextBox(root(), "Start with feedback to P2", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		b4 = new MultiWidgets::TextBox(root(), "START", MultiWidgets::TextBox::VCENTER | MultiWidgets::TextBox::HCENTER);
		
		b0->eventAddListener("interactionbegin", "start0", gw);
		b0->setCSSType("StartButton");
		b0->setStyle(style());
		int buttonWidth = b0->size().maximum();
		b0->setLocation(size().maximum() * 0.5f - 100 - 2*buttonWidth, size().minimum() - 220);
		b0->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b1->eventAddListener("interactionbegin", "start3", gw);
		b1->setCSSType("StartButton");
		b1->setStyle(style());
		b1->setLocation(size().maximum() * 0.5f - 50 - buttonWidth, size().minimum() - 220);
		b1->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b2->eventAddListener("interactionbegin", "start1", gw);
		b2->setCSSType("StartButton");
		b2->setStyle(style());
		b2->setLocation(size().maximum() * 0.5f + 50, size().minimum() - 220);
		b2->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b3->eventAddListener("interactionbegin", "start2", gw);
		b3->setCSSType("StartButton");
		b3->setStyle(style());
		b3->setLocation(size().maximum() * 0.5f + 100 + buttonWidth, size().minimum() - 220);
		b3->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		b4->eventAddListener("interactionbegin", "startGame", gw);
		b4->setCSSType("StartButton");
		b4->setStyle(style());
		b4->setLocation(size().maximum() * 0.5f - b4->width()/2, size().minimum() - 220);
		b4->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		b4->hide();

		MultiWidgets::TextBox * text1Label = new MultiWidgets::TextBox(root(), "Enter player 1 name", MultiWidgets::TextEdit::HCENTER);
		text1Label->setRotation(Nimble::Math::HALF_PI);
		text1Label->setCSSType("NameInputLabel");
		text1Label->setStyle(style());
		text1Label->setWidth(text1Label->totalTextAdvance() + 50);
		text1Label->setInputTransparent(true);
		text1Label->setLocation(size().maximum() * 0.35f, size().minimum() * 0.5f - text1Label->width()/2);
		
		MultiWidgets::TextBox * text2Label = new MultiWidgets::TextBox(root(), "Enter player 2 name", MultiWidgets::TextEdit::HCENTER);
		text2Label->setRotation(-1 * Nimble::Math::HALF_PI);
		text2Label->setCSSType("NameInputLabel");
		text2Label->setStyle(style());
		text2Label->setWidth(text1Label->totalTextAdvance() + 50);
		text2Label->setInputTransparent(true);
		text2Label->setLocation(size().maximum() * 0.65f, size().minimum() * 0.5f + text1Label->width()/2);
		
		text1 = new MultiWidgets::TextEdit(root(), 0, MultiWidgets::TextEdit::HCENTER);
		//text1->setKeyboardType("fi");
		text1->Widget::setSize(240, 70);
		text1->setRotation(Nimble::Math::HALF_PI);
		text1->setLocation(size().maximum() * 0.3f, size().minimum() * 0.5f - 120);
		text1->setCSSType("NameInput");
		text1->setStyle(style());
		text1->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		text2 = new MultiWidgets::TextEdit(root(), 0, MultiWidgets::TextEdit::HCENTER);
		//text2->setKeyboardType("fi");
		text2->Widget::setSize(240, 70);
		text2->setRotation(-1 * Nimble::Math::HALF_PI);
		text2->setLocation(size().maximum() * 0.7f, size().minimum() * 0.5f + 120);
		text2->setCSSType("NameInput");
		text2->setStyle(style());
		text2->setInputFlags(MultiWidgets::Widget::INPUT_USE_TAPS);
		
		text1->hideKeyboard();
		text1->hide();
		text1Label->hide();
		text2->hideKeyboard();
		text2->hide();
		text2Label->hide();
		
		gw->b0 = b0;
		gw->b1 = b1;
		gw->b2 = b2;
		gw->b3 = b3;
		gw->b4 = b4;
		gw->text1 = text1;
		gw->text2 = text2;
		gw->text1Label = text1Label;
		gw->text2Label = text2Label;
		gw->p1 = p1;
		gw->p2 = p2;
		gw->winnerLabel = winnerLabel;
		
		gw->initBluetooth();
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
