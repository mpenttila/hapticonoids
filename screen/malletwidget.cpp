#include <MultiWidgets/ImageWidget.hpp>

#include "malletwidget.hpp"

#include <iostream>

MalletWidget::MalletWidget(MultiWidgets::Widget * parent) : MultiWidgets::ImageWidget(parent), inputcount(0) {
  //setInputFlags(inputFlags() & ~INPUT_PASS_TO_CHILDREN);
	parent = parent;
}

void MalletWidget::input(MultiWidgets::GrabManager & gm, float dt)
{
	//std::cout << "Mallet velocity: " << velocity() << std::endl;
	MultiWidgets::ImageWidget::input(gm, dt);
}
