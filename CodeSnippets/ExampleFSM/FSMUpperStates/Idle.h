/*
 * Idle.h
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMUPPERSTATES_IDLE_H_
#define EXAMPLEFSM_FSMUPPERSTATES_IDLE_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class Idle: public ExampleBaseState {
public:
	void entry() override;
	void exit() override;

	bool start() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMUPPERSTATES_IDLE_H_ */
