/**
 * @date 18.12.2016
 * @author christoph
 * @file ITopic.cpp
 * 
 *  
 * 
 */

#include <ITopic.h>


void ITopic::serialize(MsgPack* mp) {
	_serialize(mp);
}

void ITopic::deserialize(MPNode* node) {
	_deserialize(node);
}

void TopicVector::add(ITopic* topic) {
	if(ESP.getFreeHeap() > sizeof(elementTopic)){
		elementTopic* newTopic = new elementTopic;
		newTopic->ptrTopic = topic;

		if(size == 0){
			firstTopic 	= newTopic;
			lastTopic 	= newTopic;
		}
		else{
			lastTopic->nextElement = newTopic;
			lastTopic = newTopic;
		}
		size++;
	}
}

void TopicVector::remove(ITopic* topic) {
	elementTopic* it = firstTopic;
	if(it->ptrTopic->topic == topic->topic){
		firstTopic = it->nextElement;
		delete(it);
	}
	else{
		while(it != lastTopic){
			if(it->nextElement->ptrTopic->topic == topic->topic){
				if(it->nextElement == lastTopic){
					lastTopic = it;
					delete(it->nextElement);
					it->nextElement = nullptr;
				}else{
					elementTopic* temp = it->nextElement;
					it->nextElement = it->nextElement->nextElement;
					delete(temp);
					temp = nullptr;
				}
				break;
			}
			else{
				it = it->nextElement;
			}
		}
	}
}

ITopic* TopicVector::findById(uint64_t id) {
	elementTopic* it = firstTopic;
	if(it == nullptr){
		return nullptr;
	}
	if(it->ptrTopic->id == id){
		return it->ptrTopic;
	}
	while(it != lastTopic){
		if(it->ptrTopic->id == id){
			return it->ptrTopic;
		}
		else{
			it = it->nextElement;
		}
	}
	return nullptr;
}

ITopic* TopicVector::findByName(String uri) {
	elementTopic* it = firstTopic;
	if(it == nullptr){
		return nullptr;
	}
	if(it->ptrTopic->topic == uri){
		return it->ptrTopic;
	}
	while(it != lastTopic){
		if(it->ptrTopic->topic == uri){
			return it->ptrTopic;
		}
		else{
			it = it->nextElement;
		}
	}
	return nullptr;
}
