/*
 * ReadyWaiting.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ReadyWaiting.h"
#include "Transport.h"
#include "Error.h"

namespace CBasics {

void ReadyWaiting::entry(){
	printf("	[COMP] |Ready_Waiting| called entry!\n");
}

void ReadyWaiting::exit(){
	printf("	[COMP] |Ready_Waiting| called exit!\n");
}


bool ReadyWaiting::itemAtBeginning(){
	printf("	[COMP] |Ready_Waiting| called itemAtBeginning!\n");

	exit();

	data->addItem();
	new(this) Transport;
	entry();

	return true;
}

bool ReadyWaiting::itemAtHeightSensor(){
	printf("	[COMP] |Ready_Waiting| called itemAtHeightSensor and sends an error! -> |Error|\n");

	exit();
	new(this) Error;
	entry();

	return true;
}

} /* namespace CBasics */
