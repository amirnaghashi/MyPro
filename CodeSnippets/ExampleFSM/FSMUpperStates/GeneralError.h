/*
 * GeneralError.h
 *
 *  Created on: 24.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_FSMUPPERSTATES_GENERALERROR_H_
#define EXAMPLEFSM_FSMUPPERSTATES_GENERALERROR_H_

#include "../ExampleBaseState.h"

namespace CBasics {

class GeneralError: public ExampleBaseState {
public:
	void entry() override;
	void exit() override;
	void entryStartNode() override;
	void handleDefaultExit(bool& handled) override;
	void endNode() override;

	bool errorAck() override;
	bool errorHandled() override;
	bool errorFixed() override;
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_FSMUPPERSTATES_GENERALERROR_H_ */
