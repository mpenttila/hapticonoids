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

#include <MultiWidgets/ImageWidget.hpp>
#include <MultiWidgets/SimpleSDLApplication.hpp>
#include <MultiWidgets/StayInsideParentOperator.hpp>

#include <Radiant/Directory.hpp>

/* This simple application loads images from a directory, and shows
   them on the screen. */

int main(int argc, char ** argv)
{
  SDL_Init(SDL_INIT_VIDEO);

  MultiWidgets::SimpleSDLApplication app;

  if(!app.simpleInit(argc, argv))
    return 1;

  float imagesize = app.size().minimum() * 0.1f;
  float imagestep = app.size().minimum() * 0.50f;

   // Create an image widget:
   MultiWidgets::ImageWidget * w =
     new MultiWidgets::ImageWidget();

   /* Check that the file is loadable. The actual file loading is
      done in a background thread, so that the loading does not stall
      the application. */
   if(w->load(std::string("mallet.png"))) {

     // Resize the Widget:
     w->resizeToFit(Nimble::Vector2(imagesize, imagesize));
     // Set location
     w->setLocation(Nimble::Vector2(imagestep, imagestep));
     /* Make the image widgets stay inside the parent widget. */
     w->addOperator(new MultiWidgets::StayInsideParentOperator());
     app.root()->addChild(w);
   }
   else {
	Radiant::error("Could not load image file mallet.png");
     delete w;
   }

  // Run the application:
  return app.run();
}
