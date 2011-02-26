#include "contactlistener.hpp"
#include <iostream>
 
ContactListener::ContactListener() : _contacts() {
}
 
ContactListener::~ContactListener() {
}
 
void ContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(myContact);
	//std::cout << "CONTACT!" << std::endl;
	// This might be the place for bluetooth hook
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