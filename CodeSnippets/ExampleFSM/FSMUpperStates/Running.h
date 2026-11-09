/*
 * Running.h
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMUPPERSTATES_RUNNING_H_
#define EXAMPLEFSM_FSMUPPERSTATES_RUNNING_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class Running: public ExampleBaseState {
public:
	void entry() override;
	void exit() override;
    virtual void entryStartNode();
    virtual void entryHistory();

	virtual bool itemAtBeginning() override;
	virtual bool itemAtHeightSensor() override;
	virtual bool itemMissing() override;
	virtual bool itemFound() override;
	virtual bool errorDetected() override;
	virtual bool stop() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMUPPERSTATES_RUNNING_H_ */
