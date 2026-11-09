/*
 * ReadyWaiting.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_WAITFORFIX_H_
#define EXAMPLEFSM_FSMSTATES_WAITFORFIX_H_

#include "../../ExampleBaseState.h"

namespace CBasics {

class WaitForFix: public ExampleBaseState {
public:
	void entry() override;

	bool errorFixed() override;
	bool errorAck() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_WAITFORFIX_H_ */
