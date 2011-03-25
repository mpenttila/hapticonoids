#include <iostream>
#include <new>
#include "hapticfeedback.hpp"

HapticFeedback::HapticFeedback() {
}

HapticFeedback::~HapticFeedback() {

    vector<int>::iterator i;
    for ( i = clients.begin(); i < clients.end(); i++) {
        close(*i);
    }    
}

bool HapticFeedback::init(){
	memset(&loc_addr, 0, sizeof(loc_addr));
    memset(&rem_addr, 0, sizeof(rem_addr));
    serverSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(serverSocket < 0){
		return false;
	}
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = (bdaddr_t) {{0, 0, 0, 0, 0, 0}};
    loc_addr.rc_channel = (uint8_t) 11;
    bind(serverSocket, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    listen(serverSocket, 1); 
    return true;
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

sdp_session_t *HapticFeedback::registerService() {

    uint32_t service_uuid_int[] = { 0x1998ea20, 0xcfca4619, 0x837db36b, 0x04fde3d5 };
    //uint32_t service_uuid_int[] = { 0x5, 0, 0, 0x1AA1 };
    //uint32_t service_uuid_int[] = { 0xabba };
    uint8_t rfcomm_channel = 11;
    const char *service_name = "Hapticonoids haptic server!";
    const char *service_dsc = "Haptic feedback provider";
    const char *service_prov = "Hapticonoids";

    uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
    sdp_list_t *l2cap_list = 0, 
               *rfcomm_list = 0,
               *root_list = 0,
               *proto_list = 0, 
               *access_proto_list = 0;
    sdp_data_t *channel = 0, *psm = 0;

    cout << "Service UUID: " << service_uuid_int << endl;

    sdp_record_t *record = sdp_record_alloc();

    // set the general service ID
    sdp_uuid128_create( &svc_uuid, &service_uuid_int );
    sdp_set_service_id( record, svc_uuid );
    
    // set the Service class ID
    sdp_list_t service_class = {NULL, &svc_uuid};
    sdp_set_service_classes( record, &service_class);

    // make the service record publicly browsable
    sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
    root_list = sdp_list_append(0, &root_uuid);
    sdp_set_browse_groups( record, root_list );

    // set l2cap information
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    l2cap_list = sdp_list_append( 0, &l2cap_uuid );
    proto_list = sdp_list_append( 0, l2cap_list );

    // set rfcomm information
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
    rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
    sdp_list_append( rfcomm_list, channel );
    sdp_list_append( proto_list, rfcomm_list );

    // attach protocol information to service record
    access_proto_list = sdp_list_append( 0, proto_list );
    sdp_set_access_protos( record, access_proto_list );

    // set the name, provider, and description
    sdp_set_info_attr(record, service_name, service_prov, service_dsc);

    int err = 0;
    sdp_session_t *session = 0;

    // connect to the local SDP server, register the service record, and 
    // disconnect
    session = sdp_connect( BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY );
    
    if(session == 0) return 0;
    
    err = sdp_record_register(session, record, 0);

    // cleanup
    //sdp_data_free( channel );
    //sdp_list_free( l2cap_list, 0 );
    //sdp_list_free( rfcomm_list, 0 );
    //sdp_list_free( root_list, 0 );
    //sdp_list_free( access_proto_list, 0 );

    return session;
}

void HapticFeedback::unregisterService(sdp_session_t *session) {
    sdp_close(session);
}



