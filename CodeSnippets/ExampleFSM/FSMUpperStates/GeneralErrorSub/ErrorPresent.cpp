/*
 * ReadyWaiting.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ErrorPresent.h"
#include "WaitForAck.h"
#include "WaitForHandle.h"


namespace CBasics {

void ErrorPresent::entry(){
	printf("	[ORTH] |ErrorPresent| called entry!\n");
}

bool ErrorPresent::errorAck(){
	printf("	[ORTH] |ErrorPresent| called errorAck!\n");
	data->setErrorAcked(true);
	exit();
	new(this) WaitForHandle;
	entry();
	return true;
}

bool ErrorPresent::errorHandled(){
	printf("	[ORTH] |ErrorPresent| called errorHandled!\n");
	data->setErrorHandled(true);
	exit();
	new(this) WaitForAck;
	entry();

	return true;
}

} /* namespace CBasics */
