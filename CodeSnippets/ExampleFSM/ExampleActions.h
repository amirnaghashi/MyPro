/*
 * ExampleActions.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_EXAMPLEACTIONS_H_
#define EXAMPLEFSM_EXAMPLEACTIONS_H_

#include <sys/neutrino.h>
#include <stdio.h>

namespace CBasics {

class ExampleActions {
private:
	int dispatcherConID;
public:
	ExampleActions(int dispatcherID);
	ExampleActions();
	virtual ~ExampleActions();

	void redLightOn();
	void redLightOff();
	void greenLightOn();
	void greenLightOff();
	void yellowLightOn();
	void yellowLightOff();
private:
//	void sendPulseToDispatcher(int eventCode, int prio, int payload);
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_EXAMPLEACTIONS_H_ */
