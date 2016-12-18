/**
 * @date 26.11.2016
 * @author christoph
 * @file WAMPEDuino.h
 * 
 *  
 * 
 */

#ifndef WAMPEDUINO_H_
#define WAMPEDUINO_H_
#define USE_SERIAL Serial
#include <string.h>
#include "Arduino.h"
#include "WP_Status.h"
#include "ITopic.h"
#include "WebSockets.h"
#include "WebSocketsClient.h"
#include "MsgPack.h"
#include "MsgUnpack.h"
using namespace std;




class WAMPEDuino {
private:
	typedef void (*WAMPEDuinoEvent)();
	WAMPEDuinoEvent onWebSocketConnected;
	WebSocketsClient *client;
	uint32_t requestCount;
	MsgPack mp;
	const char* host;
	uint32_t port;
	TopicVector subscribedTopics;
	TopicVector publishingTopics;
	String realm;

	//IWampTopicPtr findTopicBySubscriptionId(uint64_t subscriptionID);

	typedef enum wampMessage {
	    WAMP_MSG_HELLO =1,
	    WAMP_MSG_WELCOME = 2,
	    WAMP_MSG_ERROR = 8,
	    WAMP_MSG_PUBLISH = 16,
	    WAMP_MSG_SUBSCRIBE = 32,
	    WAMP_MSG_SUBSCRIBED = 33,
	    WAMP_MSG_EVENT = 36,
	    WAMP_MSG_CALL = 48,
	    WAMP_MSG_RESULT = 50,
	    WAMP_MSG_REGISTER = 64,
	    WAMP_MSG_REGISTERED = 65,
	    WAMP_MSG_INVOCATION = 68,
	    WAMP_MSG_YIELD = 70
	}wampMessage;

	WP_Status sendHelloMsg();
	WP_Status sendAbortMsg();
	WP_Status sendGoodbyeMsg();
	WP_Status sendErrorMsg();

	WP_Status sendPublishMsg(ITopic*  topic);
	WP_Status sendSubcribeMsg(ITopic*  topic);
	WP_Status sendUnsubscribeMsg(ITopic*  topic);
	typedef enum wampState {
		WAMP_WS_Not_Connected,
		WAMP_WS_Connecting,
		WAMP_WS_Connected,
		WAMP_Not_Connected,
		WAMP_Connecting,
		WAMP_Connected
	}wampState;
	wampState state;

	void freshPub();
public:

	WAMPEDuino();
	virtual ~WAMPEDuino();

	WP_Status begin(WebSocketsClient *client, const char* host, int32_t port, String realm, WAMPEDuinoEvent onWebSocketConnected);
	WP_Status loop();

	WP_Status subscribe(ITopic* topic);
	WP_Status publish(ITopic* topic);
	WP_Status webSocketEventHandler(WStype_t type, uint8_t * payload, size_t lenght);

	WP_Status decodeMessage(uint8_t * payload, size_t lenght);

	static void onEvent(WStype_t type, uint8_t * payload, size_t length);
	static WAMPEDuino* object;
};

#endif /* WAMPEDUINO_H_ */
