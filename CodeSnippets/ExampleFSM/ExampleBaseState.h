/*
 * ExampleBaseState.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_EXAMPLEBASESTATE_H_
#define EXAMPLEFSM_EXAMPLEBASESTATE_H_

#include <stdio.h>
#include <new>
#include <vector>
#include "ExampleData.h"
#include "ExampleActions.h"

namespace CBasics {



class ExampleContext;

class ExampleBaseState {
protected:
	ExampleData* data;
	ExampleActions* actions;


	ExampleBaseState* subStateRunning;

	ExampleBaseState* generalErrorSubHandling;
	ExampleBaseState* generalErrorSubFixing;

	bool endState = false;

public:
	int x;
	virtual ~ExampleBaseState(){};
public:
	void setContextData(ExampleData* data);
	void setActions(ExampleActions* actions);

	void initSubStates();
	virtual void handleDefaultExit(bool& handled);

	virtual void entry();
	virtual void exit();

    virtual void entryStartNode();
    virtual void entryHistory();
    virtual void endNode();
    virtual bool isEndState();
    virtual void setEndState(bool val);

	// possible trigger list for concrete states
	virtual bool itemAtBeginning();
	virtual bool itemAtHeightSensor();
	virtual bool itemMissing();
	virtual bool itemFound();
	virtual bool errorDetected();
	virtual bool start();
	virtual bool stop();
	virtual bool errorAck();
	virtual bool errorHandled();
	virtual bool errorFixed();

};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_EXAMPLEBASESTATE_H_ */
