/*
 * ExampleData.h
 *
 *  Created on: 10.05.2021
 *      Author: Megaport
 */

#ifndef EXAMPLEFSM_EXAMPLEDATA_H_
#define EXAMPLEFSM_EXAMPLEDATA_H_

namespace CBasics {

class ExampleData {
private:
	int itemCnt;
	bool errorHandled;
	bool errorAcked;
public:
	ExampleData();
	virtual ~ExampleData();

	void addItem();
	void removeItem();
	int getItemCnt();
	void setErrorHandled(bool);
	bool getErrorHandled();
	void setErrorAcked(bool);
	bool getErrorAcked();
};

} /* namespace CBasics */

#endif /* EXAMPLEFSM_EXAMPLEDATA_H_ */
