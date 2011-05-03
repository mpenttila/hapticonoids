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
#include <MultiWidgets/SimpleSDLApplication.hpp>
#include <MultiWidgets/StayInsideParentOperator.hpp>

#include <Radiant/Directory.hpp>

#define MALLET1_VERTICAL_FRACTION 0.15f
#define MALLET2_VERTICAL_FRACTION 0.85f

int main(int argc, char ** argv)
{
  SDL_Init(SDL_INIT_VIDEO);

  MultiWidgets::SimpleSDLApplication app;

  if(!app.simpleInit(argc, argv))
    return 1;

  app.setStyleFile("style.css");

  MultiWidgets::ImageWidget * background = new MultiWidgets::ImageWidget();
  background->load(std::string("air-hockey.png"));
  background->resizeToFit(app.root()->size());
  background->setDepth(-30);
  background->setInputTransparent(true);
  app.root()->addChild(background);

  // Create physics / game widget

  AirHockeyWidget * gw = new AirHockeyWidget(app.root());
  gw->setSize(app.root()->size());
  gw->setStyle(app.style());
  gw->setDepth(0);
  gw->setInputTransparent(true);
  gw->setColor(0, 0, 0, 0);

  float malletsize = app.size().minimum() * 0.1f;
  float pucksize = app.size().minimum() * 0.07f;
  float vertical_middle = app.size().minimum() * 0.5f;
  
  float mallet1_horizontal = app.size().maximum() * MALLET1_VERTICAL_FRACTION;
  float mallet2_horizontal = app.size().maximum() * MALLET2_VERTICAL_FRACTION;
  float puck_horizontal = app.size().maximum() * 0.5f;

  // Create image widgets:
  MultiWidgets::ImageWidget * mallet1 = new MultiWidgets::ImageWidget();
  MultiWidgets::ImageWidget * mallet2 = new MultiWidgets::ImageWidget();
  MultiWidgets::ImageWidget * puck = new MultiWidgets::ImageWidget();  

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

   gw->scorewidget = new MultiWidgets::TextBox(app.root(), 0, MultiWidgets::TextBox::HCENTER);
   gw->scorewidget->setStyle(app.style());
   gw->scorewidget->setText(std::string("0    0"));
   int scorewidth = gw->scorewidget->totalTextAdvance() + 300;
   gw->scorewidget->setWidth(scorewidth);
   gw->scorewidget->setInputTransparent(true);
   gw->scorewidget->setColor(0, 0, 0, 0);
   gw->scorewidget->setLocation(app.size().maximum() * 0.5f - scorewidth/2 - 5, app.size().minimum() * 0.1f);
   
   MultiWidgets::TextBox * p1 = new MultiWidgets::TextBox(app.root(), 0, MultiWidgets::TextBox::HCENTER);
   p1->setStyle(app.style());
   p1->setText(std::string("Player 1"));
   p1->setWidth(p1->totalTextAdvance() + 50);
   p1->setInputTransparent(true);
   p1->setColor(0, 0, 0, 0);
   p1->setRotation(Nimble::Math::HALF_PI);
   p1->setLocation(app.size().maximum() * 0.1f, app.size().minimum() * 0.5f - p1->totalTextAdvance()/2 - 10);
   
   MultiWidgets::TextBox * p2 = new MultiWidgets::TextBox(app.root(), 0, MultiWidgets::TextBox::HCENTER);
   p2->setStyle(app.style());
   p2->setText(std::string("Player 2"));
   p2->setWidth(p2->totalTextAdvance() + 50);
   p2->setInputTransparent(true);
   p2->setColor(0, 0, 0, 0);
   p2->setRotation(-1 * Nimble::Math::HALF_PI);
   //p2->setLocation(app.size().maximum() * 0.9f, app.size().minimum() * 0.5f - p2->totalTextAdvance()/2 - 10));
   p2->setLocation(app.size().maximum() * 0.9f, app.size().minimum() - p2->totalTextAdvance());
   
   gw->initBluetooth();

   // Run the application:
   return app.run();
}
