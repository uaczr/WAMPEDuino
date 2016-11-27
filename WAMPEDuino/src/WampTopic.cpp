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
	_serialize(msgPack);
	return Wamp_Message_Datatype_Not_Implemented;
}

template <class T> WP_Status WampTopic<T>::deserialize(MPNode &args, MPNode &argskw){
	_deserialize(args, argskw);
	event();
	return Wamp_Message_Datatype_Not_Implemented;
}
template <class T> WP_Status WampTopic<T>::_serialize(MsgPack &msgPack){
	return Wamp_Message_Datatype_Not_Implemented;
}

template <class T> WP_Status WampTopic<T>::_deserialize(MPNode &args, MPNode &argskw){
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
void IWampTopic::calculateId(){
	randomSeed(millis());
	this->subscription_id = random(UINT64_MAX);
}
uint64_t IWampTopic::getId(){
	return this->subscription_id;
}
template <> WP_Status WampTopic<int32_t>::_serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	this->fresh = false;
	return Status_Good;
}
template <> WP_Status WampTopic<int32_t>::_deserialize(MPNode &args, MPNode &argskw){
	this->data = args.at(0);
	this->fresh = true;
	return Status_Good;
}

template <> WP_Status WampTopic<char*>::_serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	this->fresh = false;
	return Status_Good;
}
template <> WP_Status WampTopic<char*>::_deserialize(MPNode &args, MPNode &argskw){
	this->data = args.at(0);
	this->fresh = true;
	return Status_Good;
}

template <> WP_Status WampTopic<bool>::_serialize(MsgPack &msgPack){
	msgPack.pack(this->data);
	this->fresh = false;
	return Status_Good;
}
template <> WP_Status WampTopic<bool>::_deserialize(MPNode &args, MPNode &argskw){
	this->data = args.at(0);
	this->fresh = true;
	return Status_Good;
}
