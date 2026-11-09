/*
 * Transport.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMSTATES_TRANSPORT_H_
#define EXAMPLEFSM_FSMSTATES_TRANSPORT_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class Transport: public ExampleBaseState {
public:
	void entry() override;

	bool itemAtHeightSensor() override;
	bool itemAtBeginning() override;
	bool itemMissing() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMSTATES_TRANSPORT_H_ */
