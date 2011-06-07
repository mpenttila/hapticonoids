#include <Luminous/Utils.hpp>
#include <Luminous/FramebufferObject.hpp>
#include <Nimble/Random.hpp>
#include <Box2D/Box2D.h>

#include "airhockeywidget.hpp"

#define MALLET1_VERTICAL_FRACTION 0.15f
#define MALLET2_VERTICAL_FRACTION 0.85f

AirHockeyWidget::AirHockeyWidget(MultiWidgets::Widget * parent) :
	MultiWidgets::ImageWidget(parent),
	m_world(b2Vec2(0,0), true),
	leftScore(0),
	rightScore(0),
	mallet_puck_vibration_type(this, "mallet-puck-vibration-type", 0),
	mallet_wall_vibration_type(this, "mallet-wall-vibration-type", 0),
	scoring_vibration_type(this, "scoring-vibration-type", 0),
	scoring_sound_type(this, "scoring-sound-type", 0),
	wall_hit_sound_type(this, "wall-hit-sound-type", 0),
	puck_hit_sound_type(this, "puck-hit-sound-type", 0),
	victory_sound_type(this, "victory-sound-type", 0),
	win_limit(this, "win-limit", 0),
	use_bluetooth(this, "bluetooth", 0)
	{
		setCSSType("AirHockeyWidget");
		w = h = 0;
		contactListener = new ContactListener(this);
		m_world.SetContactListener(contactListener);
		player1 = new int(P1_MALLET);
		player2 = new int(P2_MALLET);
		puckid = new int(PUCK);
	}

AirHockeyWidget::~AirHockeyWidget(){
	if(service > 0){
		hf.unregisterService(service);
	}
}

void AirHockeyWidget::initBluetooth(){
	if(use_bluetooth == 1){
		std::cout << "Connecting to Bluetooth adapter... " << std::endl;
		hf.init();
		service = hf.registerService();
		
		if(service > 0){
			std::cout << "Bluetooth adapter found." << std::endl;
		}
		else{
			std::cout << "Bluetooth adapter not found, disabling Bluetooth." << std::endl;
			use_bluetooth = 0;
			return;
		}
		
		// This will block until client is found
		std::cout << "Waiting for Bluetooth devices..." << std::endl;
		//client1 = hf.getClient();
	}
	else{
		std::cout << "Bluetooth disabled." << std::endl;
	}
}

void AirHockeyWidget::startGame(int _feedbackMode){
	feedbackMode = _feedbackMode;
	logger.startGame(feedbackName[feedbackMode], "Arska", "Jorma");
}

void AirHockeyWidget::endGame(int player){
	sendVictorySound(player);
	char buffer[50];
	sprintf(buffer, "Player %d wins!", player);
	scorewidget->setText(buffer);
	logger.endGame(player, leftScore, rightScore);
	highscore->insertScore("Arska", 30);
	rightScore = 0;
	leftScore = 0;
	highscore->displayScores();
}

void AirHockeyWidget::processMessage(const char * id, Radiant::BinaryData & data)
{
	if(strcmp(id, "start0") == 0) {
		std::cout << "start 0 pressed" << endl;
	}
	else {
		Widget::processMessage(id, data);
    }
}

void AirHockeyWidget::sendPuckHit(int player){
	if(use_bluetooth == 1){
		// int player_number, int message_type, int message_id
		hf.sendMessageToPlayer(player, 1, puck_hit_sound_type);
		hf.sendMessageToPlayer(player, 0, mallet_puck_vibration_type);
	}
}

void AirHockeyWidget::sendWallHit(int player){
	if(use_bluetooth == 1){
		// int player_number, int message_type, int message_id
		hf.sendMessageToPlayer(player, 1, wall_hit_sound_type);
		hf.sendMessageToPlayer(player, 0, mallet_wall_vibration_type);
	}
}

void AirHockeyWidget::sendScoringSoundAndVibration(int player){
	if(use_bluetooth == 1){
		// int player_number, int message_type, int message_id
		hf.sendMessageToPlayer(player, 1, scoring_sound_type);
		hf.sendMessageToPlayer(player, 0, scoring_vibration_type);
	}
}

void AirHockeyWidget::sendVictorySound(int player){
	if(use_bluetooth == 1){
		// int player_number, int message_type, int message_id
		hf.sendMessageToPlayer(player, 1, victory_sound_type);
	}
}

void AirHockeyWidget::ensureWidgetsHaveBodies() {
	
  MultiWidgets::Widget::ChildIterator it = childBegin();
  MultiWidgets::Widget::ChildIterator end = childEnd();

  while (it != end) {
    if (m_bodies.count(*it) == 0) {
      b2BodyDef bodyDef;
      bodyDef.type = b2_dynamicBody;

      Nimble::Vector2 center = it->mapToParent(0.5f*it->size());
      bodyDef.position.Set(toBox2D(center).x, toBox2D(center).y);

      bodyDef.angle = it->rotation();
      b2Body * body = m_world.CreateBody(&bodyDef);

      Nimble::Vector2 sz = 0.5f * it->size() * it->scale();
      sz.clamp(0.1f, 10000.0f);
      b2CircleShape circle;
      
	  circle.m_radius = toBox2D(sz).x;

      b2FixtureDef fixtureDef;
      fixtureDef.shape = &circle;
 	  
	  if(*it == puck){
		fixtureDef.friction = 0.9f;
		fixtureDef.restitution = 1.0f;
		fixtureDef.density = 10.0f;
		body->SetLinearDamping(0.4f);
	  }
	  else{
		fixtureDef.friction = 1.0f;
		fixtureDef.restitution = 0.5f;
		fixtureDef.density = 100.0f;
		body->SetLinearDamping(10.0f);
	  }
	
	  // Prevent rotation
	  body->SetAngularDamping(99.0f);

      b2Fixture * fixture = body->CreateFixture(&fixtureDef);
      if(*it == mallet1){
		  fixture->SetUserData(player1);
	  }
	  else if(*it == mallet2){
		  fixture->SetUserData(player2);
	  }
	  else{
		  // This is the puck
		  fixture->SetUserData(puckid);
	  }
	  
      m_bodies[*it] = body;
      m_fixtures[*it] = fixture;
    }
    ++it;
  }
}

void AirHockeyWidget::ensureGroundInitialized() {
  static bool groundInit = false;
  const float border = 0.85f;
  if (!groundInit) {
    groundInit = true;
    b2BodyDef groundDef;
    b2Vec2 c = toBox2D(size() * 0.5f);
    groundDef.position.Set(c.x, c.y);
    groundBody = m_world.CreateBody(&groundDef);
    b2PolygonShape groundBox;
    // Left upper
    groundBox.SetAsBox(border, c.y * 0.3f, b2Vec2(-c.x, -c.y * 0.7f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Left bottom
    groundBox.SetAsBox(border, c.y * 0.3f, b2Vec2(-c.x, c.y * 0.7f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Bottom
    groundBox.SetAsBox(c.x, border, b2Vec2(0.0f, c.y), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Right upper
    groundBox.SetAsBox(border, c.y * 0.3f, b2Vec2(c.x, -c.y * 0.7f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Right bottom
    groundBox.SetAsBox(border, c.y * 0.3f, b2Vec2(c.x, c.y * 0.7f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Top
    groundBox.SetAsBox(c.x, border, b2Vec2(0.0f, -c.y), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
  }
}

void AirHockeyWidget::updateBodiesToWidgets() {
  for (std::map<void*, b2Body*>::iterator it = m_bodies.begin(); it != m_bodies.end(); ++it) {
    b2Vec2 position = it->second->GetPosition();
    float angle = it->second->GetAngle();
    
    Widget * w = (Widget*)it->first;
    w->setCenterLocation(fromBox2D(position));
    w->setRotation(angle);
  }
}

void AirHockeyWidget::checkScoring(){
	Nimble::Vector2 center = puck->mapToParent(0.5f * puck->size());
	bool scored = false;
	char buffer[50];
	if(center.x <= 0){
		// Right player scores
		logger.logGoal(2);
		rightScore++;	
		if(rightScore == win_limit){
			endGame(2);
		}
		else{
			sendScoringSoundAndVibration(2);
			sprintf(buffer, "%d    %d", leftScore, rightScore);
			scorewidget->setText(buffer);
		}
		scorewidget->setText(buffer);
	}
	else if(center.x >= size().maximum()){
		// Left player scores
		logger.logGoal(1);
		leftScore++;
		if(leftScore == win_limit){
			endGame(1);
		}
		else{
			sendScoringSoundAndVibration(1);
			sprintf(buffer, "%d    %d", leftScore, rightScore);
			scorewidget->setText(buffer);
		}
	}
	// Restore all lost widgets to table
	for (std::map<void*, b2Body*>::iterator it = m_bodies.begin(); it != m_bodies.end(); ++it) {
		Widget * w = (Widget*)it->first;
		Nimble::Vector2 pos = w->mapToParent(0.5f * w->size());
		if((pos.x <= 0) || (pos.x >= size().maximum()))
		{
			if((Widget*)it->first == puck){	
				it->second->SetTransform(toBox2D(Nimble::Vector2(size().maximum() * 0.5f, size().minimum() * 0.5f)), 0);
			}
			else if((Widget*)it->first == mallet1){
				it->second->SetTransform(toBox2D(Nimble::Vector2(size().maximum() * MALLET1_VERTICAL_FRACTION, size().minimum() * 0.5f)), 0);
			}
			else if((Widget*)it->first == mallet2){
				it->second->SetTransform(toBox2D(Nimble::Vector2(size().maximum() * MALLET2_VERTICAL_FRACTION, size().minimum() * 0.5f)), 0);
			}
			it->second->SetLinearVelocity(b2Vec2(0,0));
		}
	}
}

void AirHockeyWidget::update(float dt)
{
  MultiWidgets::Widget::update(dt);
    
  m_world.Step(dt, 5, 5);
  m_world.ClearForces();

  updateBodiesToWidgets();

  // Check scoring
  checkScoring();

}

void AirHockeyWidget::input(MultiWidgets::GrabManager & gm, float dt)
{  

	ensureWidgetsHaveBodies();
	ensureGroundInitialized();

	MultiTouch::Sample s1 = gm.prevSample();
	MultiTouch::Sample s2 = gm.sample();
	std::vector<long> lost;

	for (std::set<long>::iterator it = m_currentFingerIds.begin(); it != m_currentFingerIds.end(); ) {
		if (s2.findFinger(*it).isNull()) {
			lost.push_back(*it);
			m_currentFingerIds.erase(it++);
		} else {
			it++;
		}
	}
  
	for (unsigned int i=0; i < lost.size(); ++i) {
	MultiTouch::Finger f = s1.findFinger(lost[i]);
	if (f.isNull()) continue;
	
		// Destroy joints from lost fingers
	
		if(m_fingerjoints.count(f.id()) > 0){
			b2MouseJoint * joint = m_fingerjoints[f.id()];
			m_world.DestroyJoint(joint);
			m_fingerjoints.erase(f.id());
		}
	}

	int n = gm.fingerCount();  
	for(int i = 0; i < n; i++) {
		
		MultiTouch::Finger f = gm.getFinger(i);
  
		m_currentFingerIds.insert(f.id());

		Nimble::Vector2 loc = f.tipLocation();
		Nimble::Vector2 locPrev = f.prevTipLocation();

		b2Vec2 locationWorld = toBox2D(locPrev);
		b2Vec2 newLocation = toBox2D(loc);
	
		if(m_fingerjoints.count(f.id()) > 0){
			// Update joint
			m_fingerjoints[f.id()]->SetTarget(newLocation);
		}
		else{
	
			MultiWidgets::Widget::ChildIterator it = childBegin();
			MultiWidgets::Widget::ChildIterator end = childEnd();

			while (it != end) {
				if(*it == mallet1 || *it == mallet2){
					b2Fixture * fixture = m_fixtures[*it];
					if (fixture->TestPoint(locationWorld)) {
						b2MouseJointDef md;
						md.bodyA = groundBody;
						md.bodyB = m_bodies[*it];
						md.target = newLocation;
						md.collideConnected = true;
						md.maxForce = 5000.0f * m_bodies[*it]->GetMass();
						m_fingerjoints[f.id()] = (b2MouseJoint *)m_world.CreateJoint(&md);
						m_bodies[*it]->SetAwake(true);
					}
				}
				it++;
			}
		}
	}
}
