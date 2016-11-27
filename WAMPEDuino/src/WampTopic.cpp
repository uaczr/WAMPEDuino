/**
 * @date 26.11.2016
 * @author christoph
 * @file WampTopic.cpp
 * 
 *  
 * 
 */

#include <WampTopic.h>

template <class T> WampTopic<T>::WampTopic() {
	this->data = 0;
	this->fresh = false;
	this->subscription_id = 0;
}

template <class T> WampTopic<T>::~WampTopic() {
	// TODO Auto-generated destructor stub
}

template <class T> WP_Status WampTopic<T>::serialize(MsgPack &msgPack){
	return Wamp_Message_Datatype_Not_Implemented;
}

template <class T> WP_Status WampTopic<T>::deserialize(MPNode &root){
	return Wamp_Message_Datatype_Not_Implemented;
}

template <class T> void WampTopic<T>::setData(T data){
	this->data = data;
	this->fresh = true;
}

template <class T> T WampTopic<T>::getData(){
	this->fresh = false;
	return data;
}

template <class T> bool WampTopic<T>::isFresh(){
	return this->fresh;
}
template <class T> void WampTopic<T>::calculateId(){
	this->subscription_id = randomSeed(millis());
}
template <class T> uint32_t WampTopic<T>::getId(){
	return this->subscription_id;
}
template <> WP_Status WampTopic<int>::serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	return Status_Good;
}
template <> WP_Status WampTopic<int>::deserialize(MPNode &node){
	this->data = (int)node;
	return Status_Good;
}

template <> WP_Status WampTopic<char*>::serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	return Status_Good;
}
template <> WP_Status WampTopic<char*>::deserialize(MPNode &node){
	this->data = node;
	return Status_Good;
}

template <> WP_Status WampTopic<bool>::serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	return Status_Good;
}
template <> WP_Status WampTopic<bool>::deserialize(MPNode &node){
	this->data = node;
	return Status_Good;
}
