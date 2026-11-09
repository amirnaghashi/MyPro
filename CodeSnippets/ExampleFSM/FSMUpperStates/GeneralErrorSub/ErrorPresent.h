/*
 * ReadyWaiting.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_ERRORPRESENT_H_
#define EXAMPLEFSM_FSMSTATES_ERRORPRESENT_H_

#include "../../ExampleBaseState.h"

namespace CBasics {

class ErrorPresent: public ExampleBaseState {
public:
	void entry() override;
	bool errorAck() override;
	bool errorHandled() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_ERRORPRESENT_H_ */
