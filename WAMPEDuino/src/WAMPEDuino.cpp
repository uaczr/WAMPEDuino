/**
 * @date 26.11.2016
 * @author christoph
 * @file WAMPEDuino.cpp
 * 
 *  
 * 
 */

#include "WAMPEDuino.h"

WP_Status WAMPEDuino::sendHelloMsg(string realm) {
	mp.clear();
    mp.pack_array(3);
    mp.pack((int) WAMP_MSG_HELLO);
    mp.pack(realm);
    //Details
    mp.pack_map(1);
    mp.pack("roles");
    //Roles
    mp.pack_map(1);
    mp.pack("subscriber");
    mp.pack_map(0);
    client->sendTXT(mp.getData(), mp.getUsedBuffer());
    return Status_Good;
}

WP_Status WAMPEDuino::sendAbortMsg() {

}

WP_Status WAMPEDuino::sendGoodbyeMsg() {
}

WP_Status WAMPEDuino::sendErrorMsg() {
}

WP_Status WAMPEDuino::sendPublishMsg(IWampTopicPtr topic) {
    mp.clear();
    mp.pack_array(4);
    mp.pack((int) WAMP_MSG_PUBLISH);
    mp.pack(requestCount);
    mp.pack_map(0);
    topic->serialize(mp);
    client->sendTXT(mp.getData(), mp.getUsedBuffer());
    requestCount++;
}

WP_Status WAMPEDuino::sendSubcribeMsg(IWampTopicPtr topic) {
    mp.clear();
    mp.pack_array(4);
    //#ID
    mp.pack((int) WAMP_MSG_SUBSCRIBE);
    mp.pack(requestCount);
    mp.pack_map(0);
    topic->serialize(mp);
    client->sendTXT(mp.getData(), mp.getUsedBuffer());
    requestCount++;
    return Status_Good;
}

WP_Status WAMPEDuino::sendUnsubscribeMsg(IWampTopicPtr topic) {

}

WAMPEDuino::WAMPEDuino(WebSocketsClient* client,string URL, int32_t port) {
	this->host = URL;
	this->port = port;
	this->requestCount = 0;
	this->client = client;
	this->state = WAMP_Not_Connected;
}

WAMPEDuino::~WAMPEDuino() {
	// TODO Auto-generated destructor stub
}

WP_Status WAMPEDuino::begin(string realm) {
	sendHelloMsg(realm);
	return Status_Good;
}

WP_Status WAMPEDuino::subscribe(IWampTopicPtr topic) {
	sendSubcribeMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::publish(IWampTopicPtr topic) {
	sendPublishMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::webSocketEventHandler(WStype_t type, uint8_t * payload, size_t lenght) {
	switch(type){
	case WStype_CONNECTED:
		break;
	case WStype_TEXT:
		decodeMessage(payload, lenght);
		break;
	}
	return Status_Good;
}

WP_Status WAMPEDuino::decodeMessage(uint8_t * payload, size_t lenght) {
	int msgType = 0;
	MsgUnpack munp(payload,lenght);
	MPNode root = munp.getRoot();
	msgType = root[0];


	return Status_Good;
}
