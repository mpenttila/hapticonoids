#ifndef MALLETWIDGET_H
#define MALLETWIDGET_H

#include <MultiWidgets/ImageWidget.hpp>

class MalletWidget : public MultiWidgets::ImageWidget
{
public:
  MalletWidget(MultiWidgets::Widget * parent = 0);
  virtual void input(MultiWidgets::GrabManager & gm, float dt);

/*
  static WidgetList * createNiceList(Widget * parent = 0, Widget * content = 0);
  virtual void update(float dt);
  void layout();
  void processFingers(MultiWidgets::GrabManager &gm, MultiWidgets::FingerArray &fingers, float dt);
  /// see if user is trying to split the list
  void handleSplits(MultiWidgets::GrabManager &gm, MultiWidgets::FingerArray &fingers, float dt);
  // return the bounding rectangle of widget in list in (common) parent coordinates
  /// @param part -1 means left half, 0 full size, 1 right half
  Nimble::Rectangle itemRect(const Widget * w, int part=0);
  // find item from list which contains local coordinate pos
  ItemList::iterator findItem(Nimble::Vector2 pos);
  void addItem(Widget * w);
  void renderContent(Luminous::RenderContext &r);
*/
  int inputcount;
  void * parent;
};


#endif // MALLETWIDGET_H
