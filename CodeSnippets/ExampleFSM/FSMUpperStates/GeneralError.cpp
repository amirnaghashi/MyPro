/*
 * GeneralError.cpp
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#include "GeneralError.h"
#include "Running.h"
#include "GeneralErrorSub/ErrorPresent.h"
#include "GeneralErrorSub/WaitForFix.h"


namespace CBasics {

void GeneralError::entry(){
	printf("[MAIN] |GeneralError| entry called!\n");
	actions->redLightOn();
	data->setErrorAcked(false);
	data->setErrorHandled(false);
}

void GeneralError::exit(){
	printf("[MAIN] |GeneralError| exit called!\n");
	actions->redLightOff();
}

void GeneralError::handleDefaultExit(bool& handled){
	if(!handled){
		if(this->generalErrorSubFixing->isEndState()
				&& this->generalErrorSubHandling->isEndState())
		{
			endNode();
		}
		handled = true;
	}

}

void GeneralError::entryStartNode(){

	new(this->generalErrorSubHandling) ErrorPresent;
	new(this->generalErrorSubFixing) WaitForFix;

	this->generalErrorSubHandling->entry();
	this->generalErrorSubFixing->entry();
}

void GeneralError::endNode(void){
		// RESET end state flag! IMPORTANT
		this->generalErrorSubFixing->setEndState(false);
		this->generalErrorSubHandling->setEndState(false);

		exit();
		new (this) Running;
		entry();
		entryHistory();
}


bool GeneralError::errorAck(){
	bool handled = 	this->generalErrorSubHandling->errorAck();
	handled &= this->generalErrorSubFixing->errorAck();

	handleDefaultExit(handled);

	return handled;
}
bool GeneralError::errorHandled(){
	bool handled = this->generalErrorSubHandling->errorHandled();

	handleDefaultExit(handled);

	return handled;
}

bool GeneralError::errorFixed(){
	bool handled = this->generalErrorSubFixing->errorFixed();

	handleDefaultExit(handled);

		return handled;
}


} /* namespace CBasics */
