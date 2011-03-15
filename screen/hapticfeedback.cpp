#include "hapticfeedback.hpp"

HapticFeedback::HapticFeedback() {
    memset(&loc_addr, 0, sizeof(loc_addr));
    memset(&rem_addr, 0, sizeof(rem_addr));
    serverSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(serverSocket, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    listen(serverSocket, 1); 
}

HapticFeedback::~HapticFeedback() {

    vector<int>::iterator i;
    for ( i = clients.begin(); i < clients.end(); i++) {
        close(*i);
    }    
}

int HapticFeedback::getClient() {
    int client;
    socklen_t opt = sizeof(rem_addr);
    char buf[1024] = { 0 };
    client = accept(serverSocket, (struct sockaddr *)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr, buf);
    fprintf(stderr, "Accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    clients.push_back(client);
    return client; 
}

void HapticFeedback::sendMessage(int client, int type, int id) {
    char* message = new char[sizeof(int)*2+10];
    int msgsize = sprintf(message, "%u:%u", type,id);
    write(client, message, msgsize);
    delete message;
}


