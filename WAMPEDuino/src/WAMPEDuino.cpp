/**
 * @date 26.11.2016
 * @author christoph
 * @file WAMPEDuino.cpp
 * 
 *  
 * 
 */

#include "WAMPEDuino.h"

WAMPEDuino* WAMPEDuino::object = nullptr;

WP_Status WAMPEDuino::sendHelloMsg(char* realm) {
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
	return Status_Good;
}

WP_Status WAMPEDuino::sendGoodbyeMsg() {
	return Status_Good;
}

WP_Status WAMPEDuino::sendErrorMsg() {
	return Status_Good;
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
    return Status_Good;
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
	return Status_Good;
}

WAMPEDuino::WAMPEDuino() {
	this->host = nullptr;
	this->port = 0;
	this->requestCount = 0;
	this->client = nullptr;
	this->state = WAMP_Not_Connected;
	this->object = this;
	this->onWebSocketConnected = nullptr;
}

WAMPEDuino::~WAMPEDuino() {
	// TODO Auto-generated destructor stub
}
WP_Status WAMPEDuino::begin(WebSocketsClient *client, const char* host, int32_t port, WAMPEDuinoEvent onWebSocketConnected){
	Serial.print("Beginning WebSocketsSession");
	this->host = host;
	this->port = port;
	this->requestCount = 0;
	this->client = client;
	this->state = WAMP_Not_Connected;
	this->object = this;
	this->onWebSocketConnected = onWebSocketConnected;
	client->onEvent(WAMPEDuino::onEvent);
	client->begin(host, port);

	return Status_Good;
}

WP_Status WAMPEDuino::connect(char* realm) {
	if((state == WAMP_WS_Not_Connected)){
		return WebSocket_No_Connection;
	}
	sendHelloMsg(realm);
	return Status_Good;
}

WP_Status WAMPEDuino::subscribe(IWampTopicPtr topic) {
	if((state == WAMP_WS_Not_Connected)){
		return WebSocket_No_Connection;
	}
	sendSubcribeMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::publish(IWampTopicPtr topic) {
	if((state == WAMP_WS_Not_Connected)){
		return WebSocket_No_Connection;
	}
	sendPublishMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::webSocketEventHandler(WStype_t type, uint8_t * payload, size_t lenght) {
	switch(type){
	case WStype_DISCONNECTED:
		Serial.print("Disconnected\n");
		this->state = WAMP_WS_Not_Connected;
		client->begin(host ,port);
		break;
	case WStype_CONNECTED:
		Serial.print("Connected\n");
		this->state = WAMP_WS_Connected;
		break;
	case WStype_TEXT:
		Serial.print("Text\n");
		decodeMessage(payload, lenght);
		break;
	case WStype_ERROR:
		Serial.print("Error\n");
		break;
	case WStype_BIN:
		Serial.print("Binary\n");
		break;
	}
	return Status_Good;
}

/*IWampTopicPtr WAMPEDuino::findTopicBySubscriptionId(uint64_t subscriptionID) {
	for(size_t i = 0; i < this->topics.size(); i++){
		if(topics[i]->getId()==subscriptionID){
			return topics[i];
		}
	}
	return nullptr;
}*/

WP_Status WAMPEDuino::decodeMessage(uint8_t * payload, size_t lenght) {
	int msgType = 0;
	MsgUnpack munp((char*)payload,lenght);
	MPNode root = munp.getRoot();
	msgType = root.at(0);

	if(msgType == WAMP_MSG_EVENT){

		uint64_t id = root.at(1);
		MPNode args = root.at(4);
		MPNode argskw = root.at(5);
		//IWampTopicPtr topic = findTopicBySubscriptionId(id);
		//topic->deserialize(args, argskw);
		return Status_Good;
	}
	if(msgType == WAMP_MSG_WELCOME){
		this->state = WAMP_Connected;
		return Status_Good;
	}

	if(msgType == WAMP_MSG_SUBSCRIBED){
		this->state = WAMP_Connected;
		return Status_Good;
	}


	return Status_Good;
}

WP_Status WAMPEDuino::loop() {
	client->loop();
	return Status_Good;
}

void WAMPEDuino::onEvent(WStype_t type, uint8_t * payload, size_t length){
	WAMPEDuino::object->webSocketEventHandler(type, payload,length);
}
