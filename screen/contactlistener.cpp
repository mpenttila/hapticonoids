#include "contactlistener.hpp"
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
	int userA = 0, userB = 0;
	if(contact->GetFixtureA()->GetUserData() != 0){
		userA = *(int*)(contact->GetFixtureA()->GetUserData());
		widget->sendBluetoothHit(userA);
	}
	if(contact->GetFixtureB()->GetUserData() != 0){
		userB = *(int*)(contact->GetFixtureB()->GetUserData());
		widget->sendBluetoothHit(userB);
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
