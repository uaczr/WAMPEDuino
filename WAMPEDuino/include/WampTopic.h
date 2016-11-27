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
protected:
	string topic;
public:
	IWampTopic(string topic){
		this->topic = topic;
	}
	virtual ~IWampTopic() = 0;
	virtual WP_Status serialize(MsgPack &msgPack) = 0;
	virtual WP_Status deserialize(MPNode &node) = 0;
};
typedef IWampTopic* IWampTopicPtr;

template <class T> class WampTopic : public IWampTopic{
protected:
	/**
	 * Refreshindicator
	 */
	bool fresh;
	uint64_t subscription_id;
	/**
	 * Data
	 */
	T data;
public:
	WampTopic();
	WampTopic(string topic, T data):IWampTopic(topic){
		this->data = data;
		this->fresh = true;
		this->subscription_id = 0;
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
	WP_Status deserialize(MPNode &node);

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
	void calculateId();
	uint32_t getId();

};
template <> WP_Status WampTopic<int>::serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<int>::deserialize(MPNode &node);

template <> WP_Status WampTopic<bool>::serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<bool>::deserialize(MPNode &node);

template <> WP_Status WampTopic<string>::serialize(MsgPack &msgPack);
template <> WP_Status WampTopic<string>::deserialize(MPNode &node);


#endif /* INCLUDE_WAMPTOPIC_H_ */
