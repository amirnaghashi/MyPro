/*
 * ReadyWaiting.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "WaitForAck.h"
#include "WaitForHandle.h"

namespace CBasics {

void WaitForAck::entry(){
	printf("	[ORTH] |WaitForAck| called entry!\n");
}

void WaitForAck::exit(){
	printf("	[ORTH] |WaitForAck| called exit!\n");
}

bool WaitForAck::errorAck(){

	printf("	[ORTH] |WaitForAck| called errorAck!\n");
	this->data->setErrorAcked(true);
	this->setEndState(true);
	exit();

	return false; // return false, because no new state was entered therefore the transition is incomplete

}




} /* namespace CBasics */
