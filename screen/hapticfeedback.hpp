#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <vector>
#define VIBRATE 0
#define SOUND 1

using namespace std;

class HapticFeedback {
    private:
        vector<int> clients;
        int serverSocket;
        sockaddr_rc loc_addr;
        sockaddr_rc rem_addr;
 
    public:
		HapticFeedback();
        ~HapticFeedback();	

		bool init();
        int getClient();
        void sendMessage(int client, int type, int id);

        sdp_session_t *registerService();
        void unregisterService(sdp_session_t *session);

};  

