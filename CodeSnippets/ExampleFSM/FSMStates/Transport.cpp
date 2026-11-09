/*
 * Transport.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "Transport.h"
#include "ReadyWaiting.h"
#include "Error.h"

namespace CBasics {

void Transport::entry(){
	printf("	[COMP] |Transport| called entry!\n");
}

bool Transport::itemAtHeightSensor(){
	printf("	[COMP] |Transport| called itemAtHeightSensor!\n");

	data->removeItem();

	if (data->getItemCnt() == 0){
		printf("	[COMP] |Transport| called itemAtHeightSensor to |ReadyWaiting|!\n");
		exit();
		new(this) ReadyWaiting;
		entry();
	}

	return true;
}

bool Transport::itemAtBeginning(){
	printf("	[COMP] |Transport| called itemAtBeginning!\n");

	data->addItem();
	return true;
}

bool Transport::itemMissing(){
	printf("	[COMP] |Transport| called itemMissing to Error!\n");

	exit();
	new(this) Error;
	entry();

	return true;
}

} /* namespace CBasics */
