#ifndef PUCKWIDGET_H
#define PUCKWIDGET_H

#include <MultiWidgets/ImageWidget.hpp>

class PuckWidget : public MultiWidgets::ImageWidget
{
public:
  PuckWidget(MultiWidgets::Widget * parent = 0);
  virtual void input(MultiWidgets::GrabManager & gm, float dt);
};


#endif // PUCKWIDGET_H
