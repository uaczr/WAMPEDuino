/**
 * @date 18.12.2016
 * @author christoph
 * @file IntegerTopic.h
 * 
 *  
 * 
 */

#ifndef INCLUDE_INTEGERTOPIC_H_
#define INCLUDE_INTEGERTOPIC_H_

#include <ITopic.h>


class IntegerTopic: public ITopic {
private:
	int data;
protected:
	void _serialize(MsgPack* mp);
	void _deserialize(MPNode* node);
public:
	IntegerTopic(String topic):ITopic(topic){
		this->data = 0;
	};
	IntegerTopic(String topic, int data):ITopic(topic){
		this->data = data;
	};
	virtual ~IntegerTopic();
	void setData(int data);
	int getData(void);
};


#endif /* INCLUDE_INTEGERTOPIC_H_ */
