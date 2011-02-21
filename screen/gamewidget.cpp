#include <Luminous/Utils.hpp>
#include <Luminous/FramebufferObject.hpp>
#include <Nimble/Random.hpp>
#include <Box2D/Box2D.h>

#include "gamewidget.hpp"


GameWidget::GameWidget(MultiWidgets::Widget * parent) :
  MultiWidgets::ImageWidget(parent),
  m_world(b2Vec2(0,0), true)
{
  setCSSType("GameWidget");
  w = h = 0;
  contactListener = new ContactListener();
  m_world.SetContactListener(contactListener);  
}

void GameWidget::ensureWidgetsHaveBodies() {
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
		fixtureDef.friction = 0.5f;
		fixtureDef.restitution = 0.8f;
		fixtureDef.density = 10.0f;
	  }
	  else{
		fixtureDef.friction = 0.99f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.density = 100.0f;
	  }
	
	  // Prevent rotation
	  body->SetAngularDamping(99.0f);

      b2Fixture * fixture = body->CreateFixture(&fixtureDef);
      m_bodies[*it] = body;
 	  m_fixtures[*it] = fixture;
    }
    ++it;
  }
}

void GameWidget::ensureGroundInitialized() {
  static bool groundInit = false;
  const float border = 0.5f;
  if (!groundInit) {
    groundInit = true;
    b2BodyDef groundDef;
    b2Vec2 c = toBox2D(size() * 0.5f);
    groundDef.position.Set(c.x, c.y);
    groundBody = m_world.CreateBody(&groundDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(border, c.y + border, b2Vec2(-c.x, 0.0f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    groundBox.SetAsBox(c.x, border, b2Vec2(0.0f, c.y), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    groundBox.SetAsBox(border, c.y + border, b2Vec2(c.x, 0.0f), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    groundBox.SetAsBox(c.x, border, b2Vec2(0.0f, -c.y), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
  }
}

void GameWidget::updateBodiesToWidgets() {
  for (std::map<void*, b2Body*>::iterator it = m_bodies.begin(); it != m_bodies.end(); ++it) {
    b2Vec2 position = it->second->GetPosition();
    float angle = it->second->GetAngle();
    
    Widget * w = (Widget*)it->first;
    w->setCenterLocation(fromBox2D(position));
    w->setRotation(angle);
  }
}

void GameWidget::update(float dt)
{
  MultiWidgets::Widget::update(dt);
    
  m_world.Step(dt/1, 10, 10);
  m_world.ClearForces();

  updateBodiesToWidgets();

  scorewidget->setText(std::string("5   2"));

}

void GameWidget::input(MultiWidgets::GrabManager & gm, float dt)
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
