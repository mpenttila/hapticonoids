#ifndef AIRHOCKEYWIDGET_H
#define AIRHOCKEYWIDGET_H

#include <MultiWidgets/ImageWidget.hpp>
#include <MultiWidgets/TextBox.hpp>
#include <Radiant/Grid.hpp>
#include <Luminous/Utils.hpp>
#include <Box2D/Box2D.h>

#include <map>

#include "contactlistener.hpp"
#include "hapticfeedback.hpp"
#include "logger.hpp"

#define P1_MALLET 1
#define P2_MALLET 2
#define PUCK 3

#define FEEDBACK_OFF 0
#define FEEDBACK_BOTH 1
#define FEEDBACK_P1 2
#define FEEDBACK_P2 3
#define feedbackName (const char*[4]){"haptic feedback off", "haptic feedback to both", "haptic feedback to P1", "haptic feedback to P2" }

class ContactListener;

namespace {
  float BOX2D_SCALE = 50.0f;
}
class AirHockeyWidget : public MultiWidgets::ImageWidget {
  // ensure appropriate scale for box2d
  static Nimble::Vector2 fromBox2D(const b2Vec2 & v) { return Nimble::Vector2(v.x, v.y)*BOX2D_SCALE; }
  static b2Vec2 toBox2D(const Nimble::Vector2 & v) { return b2Vec2(v.x/BOX2D_SCALE, v.y/BOX2D_SCALE); }
  
private:
	int feedbackMode;
  
public:

  AirHockeyWidget(MultiWidgets::Widget * parent = 0);
  ~AirHockeyWidget();

  void update(float dt);
  void ensureWidgetsHaveBodies();
  void ensureGroundInitialized();
  void updateBodiesToWidgets();
  void checkScoring();
  void initBluetooth();
  void sendPuckHit(int player);
  void sendWallHit(int player);
  void sendScoringSoundAndVibration(int player);
  void sendVictorySound(int player);

  void input(MultiWidgets::GrabManager & gm, float dt);
  
  void initGame();
  void startGame(int _feedbackMode);
  void endGame();

  int w, h;
  std::set<long> m_currentFingerIds;
  b2World m_world;
  std::map<void*, b2Body*> m_bodies;
  std::map<void*, b2Fixture*> m_fixtures;
  b2Body * groundBody; 

  MultiWidgets::ImageWidget * puck;
  MultiWidgets::ImageWidget * mallet1;
  MultiWidgets::ImageWidget * mallet2;
  ContactListener * contactListener;

  std::map<long, b2MouseJoint*> m_fingerjoints;

  MultiWidgets::TextBox * scorewidget;

  int leftScore;
  int rightScore;

  int * player1;
  int * player2;
  int * puckid;
  
  HapticFeedback hf;
  sdp_session_t* service;
  int client1;
   
  Valuable::ValueInt mallet_puck_vibration_type;
  Valuable::ValueInt mallet_wall_vibration_type;
  Valuable::ValueInt scoring_vibration_type;
  Valuable::ValueInt scoring_sound_type;
  Valuable::ValueInt puck_hit_sound_type;
  Valuable::ValueInt wall_hit_sound_type;
  Valuable::ValueInt victory_sound_type;
  Valuable::ValueInt win_limit;
  Valuable::ValueInt use_bluetooth;
  
  Logger logger;
};


#endif
