
#include <iostream>
#include "../hapticfeedback.hpp"

int main(int argc, char** argv) {

    int i = 0;
    HapticFeedback hf;
    sdp_session_t* service = hf.registerService();
    while (i < 1) {
        cout << "Waiting for client\n";
        int client = hf.getClient();
        cout << "Client found Sending message\n"; 
        hf.sendMessage(client, 0, 1);
        cout << "Vibrate message send\n\n";
        hf.sendMessage(client, 1, 1);
        cout << "Ring message send\n\n";
        i++;
    }
    hf.unregisterService(service);
}

