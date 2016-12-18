/**
 * @date 18.12.2016
 * @author christoph
 * @file IntegerTopic.cpp
 * 
 *  
 * 
 */

#include <IntegerTopic.h>



void IntegerTopic::_serialize(MsgPack* mp) {
	mp->pack(topic);
	mp->pack_array(1);
	mp->pack(this->data);
	mp->pack_map(0);
}

void IntegerTopic::_deserialize(MPNode* node) {

	MPNode args 	= node->at(4);
	this->data = args.at(0);
	Serial.printf("WAMPEDUINO Deserializing %d\n", this->data);
}

IntegerTopic::~IntegerTopic() {
	// TODO Auto-generated destructor stub
}

void IntegerTopic::setData(int data) {
	this->data = data;
}

int IntegerTopic::getData(void) {
	return data;
}
