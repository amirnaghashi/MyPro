/*
 * ExampleContext.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_EXAMPLECONTEXT_H_
#define EXAMPLEFSM_EXAMPLECONTEXT_H_

#include "ExampleBaseState.h"
#include <stdint.h>
#include "ExampleActions.h"
#include "ExampleData.h"



#define EVT_ITEM_AT_BEGINNING 0x01
#define EVT_ITEM_AT_HEIGHTSENSOR 0x02
#define EVT_ITEM_MISSING 0x03
#define EVT_ITEM_FOUND 0x04

#define EVT_START 0x10
#define EVT_STOP 0x11
#define EVT_ERROR_DETECTED 0x12
#define EVT_ERROR_ACKNOWLEDGED 0x13
#define EVT_ERROR_HANDLED 0x14
#define EVT_ERROR_FIXED 0x15

namespace CBasics {

class ExampleContext {
private:
	ExampleBaseState* state;
	ExampleActions* actions;
	ExampleData* data;


public:
	ExampleContext(ExampleActions* actions);
	virtual ~ExampleContext();

	void handleMessage(uint8_t eventCode, uint32_t payload);
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_EXAMPLECONTEXT_H_ */
