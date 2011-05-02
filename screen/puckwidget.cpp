#include <MultiWidgets/ImageWidget.hpp>

#include "puckwidget.hpp"

#include <iostream>

PuckWidget::PuckWidget(MultiWidgets::Widget * parent) : MultiWidgets::ImageWidget(parent) {
}

void PuckWidget::input(MultiWidgets::GrabManager & gm, float dt)
{
	//std::cout << "Puck velocity: " << velocity() << std::endl;
	MultiWidgets::ImageWidget::input(gm, dt);
}
