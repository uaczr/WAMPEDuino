/**
 * @date 18.12.2016
 * @author christoph
 * @file ITopic.h
 * 
 *  
 * 
 */

#ifndef INCLUDE_ITOPIC_H_
#define INCLUDE_ITOPIC_H_

#include "Arduino.h"
#include "MsgPack.h"
#include "MsgUnpack.h"

class ITopic {
	friend class TopicVector;
protected:
	bool fresh;
	uint64_t id;
	String topic;
	virtual void _serialize(MsgPack* mp)	= 0;
	virtual void _deserialize(MPNode* node)	= 0;
public:
	ITopic(String topic){
		this->topic = topic;
		randomSeed(millis());
		this->id = random(RAND_MAX);
		this->fresh = false;
	}
	virtual ~ITopic(){

	}
	void serialize(MsgPack* mp);
	void deserialize(MPNode* node);
	void setId(uint64_t id){
		this->id = id;
	};
	String getTopic(){
		return this->topic;
	}
	void setFresh(){
		this->fresh = true;
	}
	void unsetFresh(){
		this->fresh = false;
	}
	bool isFresh(){
		return this->fresh;
	}

};

typedef struct elementTopic{
	ITopic* ptrTopic;
	elementTopic* nextElement;
} elementTopic;

class TopicVector{
public:

private:


	elementTopic* firstTopic;
	elementTopic* lastTopic;
	size_t size;
public:
	TopicVector(){
		this->firstTopic 	= nullptr;
		this->lastTopic 	= nullptr;
		this->size			= 0;
	}

	void add(ITopic* topic);
	void remove(ITopic* topic);
	ITopic* findById(uint64_t id);
	ITopic* findByName(String uri);

	friend class WAMPEDuino;
};



#endif /* INCLUDE_ITOPIC_H_ */
