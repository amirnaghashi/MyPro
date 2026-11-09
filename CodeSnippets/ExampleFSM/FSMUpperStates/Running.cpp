/*
 * Running.cpp
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#include "Running.h"
#include "GeneralError.h"
#include "Idle.h"
#include "../FSMStates/ReadyWaiting.h"

namespace CBasics {

void Running::entry(){
	printf("[MAIN] |UpperRunning| entry called!\n");
	actions->yellowLightOn();
}

void Running::exit(){
	printf("[MAIN] |UpperRunning| exit called!\n");
	actions->yellowLightOff();
}


void Running::entryStartNode(){
	new(subStateRunning) ReadyWaiting;
	subStateRunning->entry();
}

void Running::entryHistory(){
	// additional logic is possible
	subStateRunning->entry();
}

bool Running::errorDetected(){
	bool handled = subStateRunning->errorDetected();

	if (!handled){
		subStateRunning->exit();
		exit();
		new (this) GeneralError;
		entry();
		entryStartNode();
		handled = true;
	}

	return handled;
}

bool Running::itemAtBeginning(){
	bool handled = subStateRunning->itemAtBeginning();
	return handled;
}

bool Running::itemAtHeightSensor(){
	bool handled = subStateRunning->itemAtHeightSensor();
	return handled;
}

bool Running::itemFound(){
	bool handled = subStateRunning->itemFound();
	return handled;
}

bool Running::itemMissing(){
	bool handled = subStateRunning->itemMissing();
	return handled;
}

bool Running::stop(){
	bool handled = subStateRunning->stop();

	if (!handled){
		subStateRunning->exit();
		exit();
		new (this) Idle;
		entry();
		handled = true;
	}

	return handled;
}
} /* namespace CBasics */
