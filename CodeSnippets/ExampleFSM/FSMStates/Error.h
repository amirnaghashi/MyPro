/*
 * Error.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_ERROR_H_
#define EXAMPLEFSM_FSMSTATES_ERROR_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class Error: public ExampleBaseState{
public:
	void entry() override;

	bool itemFound() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_ERROR_H_ */
