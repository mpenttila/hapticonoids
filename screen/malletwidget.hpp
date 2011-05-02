#ifndef MALLETWIDGET_H
#define MALLETWIDGET_H

#include <MultiWidgets/ImageWidget.hpp>

class MalletWidget : public MultiWidgets::ImageWidget
{
public:
  MalletWidget(MultiWidgets::Widget * parent = 0);
  virtual void input(MultiWidgets::GrabManager & gm, float dt);
  int inputcount;
  void * parent;
};


#endif // MALLETWIDGET_H
