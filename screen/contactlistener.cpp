#include "contactlistener.hpp"
#include "airhockeywidget.hpp"
#include <iostream>
 
ContactListener::ContactListener(AirHockeyWidget * _widget) : _contacts() {
	widget = _widget;
}
 
ContactListener::~ContactListener() {
}
 
void ContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(myContact);
    //hf.sendMessage(client, 0, 1);
	//std::cout << "CONTACT!" << std::endl;
	// Send Bluetooth message
	int fixA = 0, fixB = 0;
	if(contact->GetFixtureA()->GetUserData() != 0){
		fixA = *(int*)(contact->GetFixtureA()->GetUserData());
	}
	if(contact->GetFixtureB()->GetUserData() != 0){
		fixB = *(int*)(contact->GetFixtureB()->GetUserData());
	}
	if(fixA == P1_MALLET || fixA == P2_MALLET){
		if(fixB == P1_MALLET || fixB == P2_MALLET){
			widget->sendPuckHit(fixA);
			widget->sendPuckHit(fixB);
		}
		else if(fixB == PUCK){
			widget->sendPuckHit(fixA);
		}
		else{
			widget->sendWallHit(fixA);
		}
	}
	if(fixB == P2_MALLET || fixB == P1_MALLET){
		if(fixA == PUCK){
			widget->sendPuckHit(fixB);
		}
		else{
			widget->sendWallHit(fixB);
		}
	}
}
 
void ContactListener::EndContact(b2Contact* contact) {
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), myContact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
}
 
void ContactListener::PreSolve(b2Contact* contact, 
  const b2Manifold* oldManifold) {
}
 
void ContactListener::PostSolve(b2Contact* contact, 
  const b2ContactImpulse* impulse) {
}
