/*
 * ReadyWaiting.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "WaitForFix.h"

namespace CBasics {

void WaitForFix::entry(){
	printf("	[ORTH] |WaitForFix| called entry!\n");
}



bool WaitForFix::errorFixed(){
	printf("	[ORTH] |WaitForFix| called errorHandled!\n");
	this->setEndState(true);
	/* send EVT_ERROR_HANDLED to Dispatcher */
	printf("[EVENT] SENDING EVT_ERROR_HANDLED\n");

	exit();


	return true;
}

bool WaitForFix::errorAck(){
	if(!this->isEndState()){
		printf("	[ORTH] |WaitForFix| called errorAck!\n");
		exit();
		entry();
		return true;
	}
	return false;

}




} /* namespace CBasics */
