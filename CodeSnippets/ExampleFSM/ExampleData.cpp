/*
 * ExampleData.cpp
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#include "ExampleData.h"

namespace CBasics {

ExampleData::ExampleData() {
	// TODO Auto-generated constructor stub
	itemCnt = 0;
	errorHandled = false;
	errorAcked = false;
}

ExampleData::~ExampleData() {
	// TODO Auto-generated destructor stub
}

void ExampleData::addItem(){
	itemCnt++;
}

void ExampleData::removeItem(){
	itemCnt--;
}

int ExampleData::getItemCnt(){
	return itemCnt;
}


bool ExampleData::getErrorHandled(){
	return errorHandled;
}

void ExampleData::setErrorHandled(bool value){
	errorHandled = value;
}

bool ExampleData::getErrorAcked(){
	return errorAcked;
}

void ExampleData::setErrorAcked(bool value){
	errorAcked = value;
}



} /* namespace CBasics */
