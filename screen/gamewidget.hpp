#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <MultiWidgets/ImageWidget.hpp>
#include <MultiWidgets/TextBox.hpp>
#include <Radiant/Grid.hpp>
#include <Luminous/Utils.hpp>
#include <Box2D/Box2D.h>

#include <map>

#include "malletwidget.hpp"
#include "puckwidget.hpp"
#include "contactlistener.hpp"

namespace {
  float BOX2D_SCALE = 50.0f;
}
class GameWidget : public MultiWidgets::ImageWidget {
  // ensure appropriate scale for box2d
  static Nimble::Vector2 fromBox2D(const b2Vec2 & v) { return Nimble::Vector2(v.x, v.y)*BOX2D_SCALE; }
  static b2Vec2 toBox2D(const Nimble::Vector2 & v) { return b2Vec2(v.x/BOX2D_SCALE, v.y/BOX2D_SCALE); }
public:

  GameWidget(MultiWidgets::Widget * parent = 0);

  void update(float dt);
  void ensureWidgetsHaveBodies();
  void ensureGroundInitialized();
  void updateBodiesToWidgets();

  void input(MultiWidgets::GrabManager & gm, float dt);

  int w, h;
  std::set<long> m_currentFingerIds;
  b2World m_world;
  std::map<void*, b2Body*> m_bodies;
  std::map<void*, b2Fixture*> m_fixtures;
  b2Body * groundBody; 

  PuckWidget * puck;
  MalletWidget * mallet1;
  MalletWidget * mallet2;
  ContactListener * contactListener;

  std::map<long, b2MouseJoint*> m_fingerjoints;

  MultiWidgets::TextBox * scorewidget;

};


#endif
