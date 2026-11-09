/*
 * ExampleActions.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ExampleActions.h"
extern int errno;
namespace CBasics {

ExampleActions::ExampleActions(int dispatcherID) {
	// TODO Auto-generated constructor stub
	if((dispatcherConID = ConnectAttach(0,0,dispatcherID,0,0)) < 0){
		printf("AbstractFSM: Dispatcher ConnectAttach failed: %d!\n", errno);
	}
}

ExampleActions::ExampleActions() {
	// TODO Auto-generated constructor stub
}


ExampleActions::~ExampleActions() {
	// TODO Auto-generated destructor stub
}

void ExampleActions::greenLightOn(){
//	sendPulseToDispatcher(12, -1, 0);
	printf(">ACTION<: green light on!\n");
}

void ExampleActions::greenLightOff(){
//	sendPulseToDispatcher(12, -1, 0);
	printf(">ACTION<: green light off!\n");
}

void ExampleActions::redLightOn(){
//	sendPulseToDispatcher(14, -1, 0);
	printf(">ACTION<: red light on!\n");
}

void ExampleActions::redLightOff(){
//	sendPulseToDispatcher(14, -1, 0);
	printf(">ACTION<: red light off!\n");
}

void ExampleActions::yellowLightOn(){
//	sendPulseToDispatcher(13, -1, 0);
	printf(">ACTION<: yellow light on!\n");
}

void ExampleActions::yellowLightOff(){
//	sendPulseToDispatcher(13, -1, 0);
	printf(">ACTION<: yellow light off!\n");
}

//void ExampleActions::sendPulseToDispatcher(int event, int prio, int payload){
//	if(MsgSendPulse(dispatcherConID,prio,event,payload) < 0){
//		printf("AbstractFSM: send event to dispatcher failed: %d\n", errno);
//	}
//}

} /* namespace CBasics */
