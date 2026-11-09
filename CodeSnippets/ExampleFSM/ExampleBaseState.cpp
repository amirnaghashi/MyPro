/*
 * ExampleBaseState.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ExampleBaseState.h"
#include "FSMStates/ReadyWaiting.h"
#include "FSMUpperStates/GeneralErrorSub/ErrorPresent.h"
#include "FSMUpperStates/GeneralErrorSub/WaitForFix.h"

namespace CBasics {

void ExampleBaseState::setContextData(ExampleData* data){
	this->data = data;
}

void ExampleBaseState::setActions(ExampleActions* actions){
	this->actions = actions;
}

void ExampleBaseState::initSubStates(){
	this->subStateRunning = new ReadyWaiting();		// Initialisierung des Substates
	this->subStateRunning->setActions(actions);
	this->subStateRunning->setContextData(data);
	this->subStateRunning->x = 5;

	this->generalErrorSubHandling = new ErrorPresent();
	this->generalErrorSubHandling->setActions(actions);
	this->generalErrorSubHandling->setContextData(data);
	this->generalErrorSubHandling->x = 10;

	this->generalErrorSubFixing = new WaitForFix();
	this->generalErrorSubFixing->setActions(actions);
	this->generalErrorSubFixing->setContextData(data);

}

void ExampleBaseState::handleDefaultExit(bool& handled) {

}


// Events

bool ExampleBaseState::itemAtBeginning(){
	return false;
}
bool ExampleBaseState::itemAtHeightSensor(){
	return false;
}
bool ExampleBaseState::itemFound(){
	return false;
}
bool ExampleBaseState::itemMissing(){
	return false;
}
bool ExampleBaseState::errorDetected(){
	return false;
}
bool ExampleBaseState::start(){
	return false;
}
bool ExampleBaseState::stop(){
	return false;
}
bool ExampleBaseState::errorAck(){
	return false;
}
bool ExampleBaseState::errorHandled(){
	return false;
}
bool ExampleBaseState::errorFixed(){
	return false;
}

bool ExampleBaseState::isEndState(){
	return this->endState;
}

void ExampleBaseState::setEndState(bool val){
	this->endState = val;
}

void ExampleBaseState::entryHistory(){

}

void ExampleBaseState::entryStartNode(){

}

void ExampleBaseState::endNode(){

}

void ExampleBaseState::entry(){
	// Do something as default entry
}

void ExampleBaseState::exit(){
	// Do something as default exit
}



} /* namespace CBasics */
