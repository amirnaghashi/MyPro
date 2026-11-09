/*
 * ReadyWaiting.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_WAITFORACK_H_
#define EXAMPLEFSM_FSMSTATES_WAITFORACK_H_

#include "../../ExampleBaseState.h"

namespace CBasics {

class WaitForAck: public ExampleBaseState {
public:
	void entry() override;
	void exit() override;

	bool errorAck() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_WAITFORACK_H_ */
