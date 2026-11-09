/*
 * ReadyWaiting.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_READYWAITING_H_
#define EXAMPLEFSM_FSMSTATES_READYWAITING_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class ReadyWaiting: public ExampleBaseState {
public:
	void entry() override;
	void exit() override;

	bool itemAtBeginning() override;
	bool itemAtHeightSensor() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_READYWAITING_H_ */

