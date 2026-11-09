/*
 * Error.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "Error.h"
#include "Transport.h"

namespace CBasics {

void Error::entry(){
	printf("	[COMP] |Error| called entry!\n");
}

bool Error::itemFound(){

	exit();
	printf("	[COMP] |Error| called itemFound to |Transport|!\n");
	new(this) Transport;
	entry();

	return true;
}

} /* namespace CBasics */
