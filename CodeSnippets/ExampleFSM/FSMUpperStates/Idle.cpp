/*
 * Idle.cpp
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#include "Idle.h"
#include "Running.h"

namespace CBasics {

void Idle::entry(){
	printf("[MAIN] |UpperIdle| called entry!\n");
	actions->greenLightOn();
}

void Idle::exit(){
	printf("[MAIN] |UpperIdle| called exit!\n");
	actions->greenLightOff();
}

bool Idle::start(){

	exit();
	new (this) Running;
	entry();
	entryStartNode();

	return true;
}

} /* namespace CBasics */
