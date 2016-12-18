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

WP_Status WAMPEDuino::sendHelloMsg() {
	Serial.println("SENDING HELLOMSG");
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
    client->sendBIN((uint8_t*)mp.getData(), mp.getUsedBuffer(), false);
    Serial.write( mp.getData(), mp.getUsedBuffer());
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

WP_Status WAMPEDuino::sendPublishMsg(ITopic* topic) {
	Serial.println("SENDING PUBLISHMSG");
    mp.clear();
    mp.pack_array(6);
    mp.pack((int) WAMP_MSG_PUBLISH);
    mp.pack(requestCount);
    mp.pack_map(0);
    topic->setId(requestCount);
    topic->serialize(&mp);
    client->sendBIN((uint8*)mp.getData(), mp.getUsedBuffer());
    Serial.write( mp.getData(), mp.getUsedBuffer());
    requestCount++;
    return Status_Good;
}

WP_Status WAMPEDuino::sendSubcribeMsg(ITopic* topic) {
	Serial.println("SENDING SUBSCRIBEMSG");
    mp.clear();
    mp.pack_array(4);
    //#ID
    mp.pack((int) WAMP_MSG_SUBSCRIBE);
    mp.pack(requestCount);
    mp.pack_map(0);
    topic->setId(requestCount);
    mp.pack(topic->getTopic().c_str());
    client->sendBIN((uint8_t*)mp.getData(), mp.getUsedBuffer());
    requestCount++;
    return Status_Good;
}

WP_Status WAMPEDuino::sendUnsubscribeMsg(ITopic* topic) {
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
WP_Status WAMPEDuino::begin(WebSocketsClient *client, const char* host, int32_t port,String realm, WAMPEDuinoEvent onWebSocketConnected){
	Serial.print("Beginning WebSocketsSession");
	this->host = host;
	this->port = port;
	this->requestCount = 0;
	this->client = client;
	this->state = WAMP_Not_Connected;
	this->object = this;
	this->onWebSocketConnected = onWebSocketConnected;
	this->realm = realm;
	client->onEvent(WAMPEDuino::onEvent);
	client->begin(host, port, "/ws/", "wamp.2.msgpack");
	state = WAMP_WS_Connecting;
	return Status_Good;
}

WP_Status WAMPEDuino::subscribe(ITopic* topic) {
	if((state == WAMP_WS_Not_Connected)){
		return WebSocket_No_Connection;
	}
	subscribedTopics.add(topic);
	sendSubcribeMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::publish(ITopic* topic) {
	if((state == WAMP_WS_Not_Connected)){
		return WebSocket_No_Connection;
	}
	publishingTopics.add(topic);
	sendPublishMsg(topic);
	return Status_Good;
}

WP_Status WAMPEDuino::webSocketEventHandler(WStype_t type, uint8_t * payload, size_t lenght) {
	switch(type){
	case WStype_DISCONNECTED:
		Serial.print("WAMPEDUINO Disconnected\n");
		this->state = WAMP_WS_Not_Connected;
		//client->begin(host ,port);
		break;
	case WStype_CONNECTED:
		Serial.print("WAMPEDUINO Connected\n");
		this->state = WAMP_WS_Connected;
		break;
	case WStype_TEXT:
		Serial.print("WAMPEDUINO Text\n");

		break;
	case WStype_ERROR:
		Serial.print("WAMPEDUINO Error\n");
		break;
	case WStype_BIN:
		Serial.print("WAMPEDUINO Binary\n");
		decodeMessage(payload, lenght);
		break;
	}
	return Status_Good;
}


WP_Status WAMPEDuino::decodeMessage(uint8_t * payload, size_t lenght) {
	int msgType = 0;
	MsgUnpack munp((char*)payload,lenght);
	MPNode root = munp.getRoot();
	msgType = root.at(0);

	if(msgType == WAMP_MSG_EVENT){
		Serial.print("WAMPEDUINO EVENT\n");
		uint64_t id = root.at(1);
		ITopic* top = subscribedTopics.findById(id);
		top->deserialize(&root);
		return Status_Good;
	}
	if(msgType == WAMP_MSG_WELCOME){
		Serial.print("WAMPEDUINO Connected to WAMP Server\n");
		this->state = WAMP_Connected;
		this->onWebSocketConnected();
		return Status_Good;
	}

	if(msgType == WAMP_MSG_SUBSCRIBED){
		Serial.print("WAMPEDUINO SUBSCRIBED\n");
		uint64_t sub_id = root.at(2);
		uint64_t request_id = root.at(1);
		ITopic* top = subscribedTopics.findById(request_id);
		if(top != nullptr){
			Serial.print("WAMPEDUINO Subscription Successfull");
			top->setId(sub_id);
		}
		else{
			Serial.print("WAMPEDUINO Subscription Failed");
		}
		return Status_Good;
	}
	return Status_Good;
}

WP_Status WAMPEDuino::loop() {
	client->loop();
	switch(state){
	case WAMP_WS_Not_Connected:
		client->begin(host, port, "/ws/", "wamp.2.msgpack");
		state = WAMP_WS_Connecting;
		break;
	case WAMP_WS_Connected:
		this->sendHelloMsg();
		break;
	case WAMP_Connected:
		freshPub();
		break;
	}
	return Status_Good;
}

void WAMPEDuino::freshPub(){
	elementTopic* it = publishingTopics.firstTopic;
	elementTopic* last = publishingTopics.lastTopic;
	if(it == nullptr){
		return;
	}
	if(it->ptrTopic->isFresh()){
		publish(it->ptrTopic);
		it->ptrTopic->unsetFresh();
	}
	while(it != last){
		if(it->ptrTopic->isFresh()){
			publish(it->ptrTopic);
			it->ptrTopic->unsetFresh();
		}
		it = it->nextElement;
	}
}

void WAMPEDuino::onEvent(WStype_t type, uint8_t * payload, size_t length){
	WAMPEDuino::object->webSocketEventHandler(type, payload,length);
}
