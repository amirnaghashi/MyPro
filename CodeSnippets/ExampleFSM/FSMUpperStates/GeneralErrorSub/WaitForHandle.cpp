/*
 * ReadyWaiting.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "WaitForHandle.h"
#include "WaitForAck.h"

namespace CBasics {

void WaitForHandle::entry(){
	printf("	[ORTH] |WaitForHandle| called entry!\n");
}

void WaitForHandle::exit(){
	printf("	[ORTH] |WaitForHandle| called exit!\n");
}

bool WaitForHandle::errorHandled(){

	printf("	[ORTH] |WaitForHandle| called errorHandled!\n");
	this->data->setErrorHandled(true);
	this->setEndState(true);
	exit();
	return false; // return false, because no new state was entered therefore the transition is incomplete

}


} /* namespace CBasics */
