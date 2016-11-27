/**
 * @date 26.11.2016
 * @author christoph
 * @file WampTopic.h
 * 
 *  
 * 
 */

#ifndef INCLUDE_WAMPTOPIC_H_
#define INCLUDE_WAMPTOPIC_H_
#include <cstdlib>
#include <string>
#include "Arduino.h"
#include "WP_Status.h"
#include "MsgPack.h"
#include "MsgUnpack.h"


using namespace std;
typedef char* uri;



class IWampTopic {
public:
	typedef void (*topicEvent)();
protected:
	string topic;
	uint64_t subscription_id;
	topicEvent event;
public:

	IWampTopic(string topic, topicEvent event){
		this->topic = topic;
		this->subscription_id = 0;
		this->event = event;
	}
	virtual ~IWampTopic() = 0;
	virtual WP_Status serialize(MsgPack &msgPack) = 0;
	virtual WP_Status deserialize(MPNode &args, MPNode &argskw) = 0;
	void calculateId();
	uint64_t getId();
	void onEvent(){
		event();
	};
};
typedef IWampTopic* IWampTopicPtr;

template <class T> class WampTopic : public IWampTopic{
protected:
	/**
	 * Refreshindicator
	 */
	bool fresh;

	/**
	 * Data
	 */
	T data;
	/**
	 * @brief Serializes a The Topic for Messaging
	 * @param [in, out] msgPack		MSG Pack packer object
	 */
	WP_Status _serialize(MsgPack &msgPack);
	/**
	 * @brief Deserializes a message and refreshes data
	 * @param [in,out] msgUnpack	MSG Pack unpacker object
	 */
	WP_Status _deserialize(MPNode &args, MPNode &argskw);
public:
	WampTopic();
	WampTopic(string topic, T data, topicEvent event):IWampTopic(topic, event){
		this->data = data;
		this->fresh = true;
	}
	virtual ~WampTopic();
	/**
	 * @brief Serializes a The Topic for Messaging
	 * @param [in, out] msgPack		MSG Pack packer object
	 */
	WP_Status serialize(MsgPack &msgPack);
	/**
	 * @brief Deserializes a message and refreshes data
	 * @param [in,out] msgUnpack	MSG Pack unpacker object
	 */
	WP_Status deserialize(MPNode &args, MPNode &argskw);

	/**
	 * @brief Sets the Topic data
	 * @param [in] data		Data of type T
	 */
	void setData(T data);
	/**
	 * @brief Gets the Topic data
	 * @return				Data of type T
	 */
	T getData();
	/**
	 * @brief Checks if Topic data is fresh
	 * @return				Bool, true = fresh, false = !fresh
	 */
	bool isFresh();


};
template <> WP_Status WampTopic<int32_t>::_serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<int32_t>::_deserialize(MPNode &args, MPNode &argskw);

template <> WP_Status WampTopic<bool>::_serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<bool>::_deserialize(MPNode &args, MPNode &argskw);

template <> WP_Status WampTopic<string>::_serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<string>::_deserialize(MPNode &args, MPNode &argskw);


#endif /* INCLUDE_WAMPTOPIC_H_ */
