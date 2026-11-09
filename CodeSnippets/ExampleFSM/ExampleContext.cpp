/*
 * ExampleContext.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ExampleContext.h"
#include "FSMUpperStates/Idle.h"

namespace CBasics {

ExampleContext::ExampleContext(ExampleActions* actions): actions(actions), data(new ExampleData()){
	// TODO Auto-generated constructor stub
	state = new Idle();
	state->setContextData(data);
	state->setActions(actions);

	state->initSubStates();
	state->entry();
}

ExampleContext::~ExampleContext() {
	// TODO Auto-generated destructor stub
}

void ExampleContext::handleMessage(uint8_t eventCode, uint32_t payload){
	switch (eventCode) {
		case EVT_ITEM_AT_BEGINNING:
			state->itemAtBeginning();
			break;
		case EVT_ITEM_AT_HEIGHTSENSOR:
			state->itemAtHeightSensor();
			break;
		case EVT_ITEM_MISSING:
			state->itemMissing();
			break;
		case EVT_ITEM_FOUND:
			state->itemFound();
			break;
		case EVT_START:
			state->start();
			break;
		case EVT_STOP:
			state->stop();
			break;
		case EVT_ERROR_DETECTED:
			state->errorDetected();
			break;
		case EVT_ERROR_ACKNOWLEDGED:
			state->errorAck();
			break;
		case EVT_ERROR_HANDLED:
			state->errorHandled();
			break;
		case EVT_ERROR_FIXED:
			state->errorFixed();
			break;

		default:
			printf("Context: The event code could not be handled!  %d\n", eventCode);
	}



}

} /* namespace CBasics */
