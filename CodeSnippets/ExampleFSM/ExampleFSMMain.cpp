//#include "ExampleContext.h"
//#include <sys/neutrino.h>
//#include <thread>
//#include <iostream>
//#include "simqnxgpioapi.h"
//
//using namespace CBasics;
//
//extern int errno;
//
//int main()
//{
//
//	std::cout << "Starting Wrapper ESEP_Example!\n";
//	std::cout << std::string(60,'=') << '\n';
//
////	testClient client(1,0);
////	std::thread t1 = std::thread(&testClient::evaluateSomething, &client);
//
////	ExampleContext* exampleFSM = new ExampleContext(new ExampleActions(client.getChannelID()));
//
//
//	/*
//	 * INITIALIZATION
//	 * ================================================
//	 * state					: NULL -> Idle
//	 * subStateRunning			: NULL -> ReadyWaiting
//	 * generalErrorSubHandling	: NULL -> ErrorPresent
//	 * generalErrorSubFixing	: NULL -> WaitForFix
//	 * ================================================
//	 */
//	ExampleContext* exampleFSM = new ExampleContext(new ExampleActions());
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_START
//	 * ================================================
//	 * state					: Idle -> Running
//	 * subStateRunning			: ReadyWaiting -> ReadyWaiting
//	 * generalErrorSubHandling	: ErrorPresent
//	 * generalErrorSubFixing	: WaitForFix
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_START\n";
//	exampleFSM->handleMessage(EVT_START, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ITEM_AT_BEGINNING
//	 * ================================================
//	 * state					: Running
//	 * subStateRunning			: ReadyWaiting -> Transport
//	 * generalErrorSubHandling	: ErrorPresent
//	 * generalErrorSubFixing	: WaitForFix
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_ITEM_AT_BEGINNING\n";
//	exampleFSM->handleMessage(EVT_ITEM_AT_BEGINNING, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ITEM_AT_HEIGHTSENSOR (itemCnt == 0)
//	 * ================================================
//	 * state					: Running
//	 * subStateRunning			: Transport -> ReadyWaiting
//	 * generalErrorSubHandling	: ErrorPresent
//	 * generalErrorSubFixing	: WaitForFix
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_ITEM_AT_HEIGHTSENSOR\n";
//	exampleFSM->handleMessage(EVT_ITEM_AT_HEIGHTSENSOR, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ERROR_DETECTED
//	 * ================================================
//	 * state					: GeneralError
//	 * subStateRunning			: ReadyWaiting
//	 * generalErrorSubHandling	: ErrorPresent -> ErrorPresent
//	 * generalErrorSubFixing	: WaitForFix -> WaitForFix
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_ERROR_DETECTED\n";
//	exampleFSM->handleMessage(EVT_ERROR_DETECTED, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ERROR_ACKNOWLEDGED
//	 * ================================================
//	 * state					: GeneralError
//	 * subStateRunning			: ReadyWaiting
//	 * generalErrorSubHandling	: ErrorPresent -> WaitForHandle
//	 * generalErrorSubFixing	: WaitForFix
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_ERROR_ACKNOWLEDGED\n";
//	exampleFSM->handleMessage(EVT_ERROR_ACKNOWLEDGED, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ERROR_FIXED
//	 * ================================================
//	 * state					: GeneralError
//	 * subStateRunning			: ReadyWaiting
//	 * generalErrorSubHandling	: WaitForHandle
//	 * generalErrorSubFixing	: WaitForFix "->" FinalNode (sending EVT_ERROR_HANDLED)
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_ERROR_FIXED\n";
//	exampleFSM->handleMessage(EVT_ERROR_FIXED, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//	/*
//	 * EVENT: EVT_ERROR_HANDLED (send by |WaitForFix| when doing the transition)
//	 * ================================================
//	 * state					: GeneralError
//	 * subStateRunning			: ReadyWaiting
//	 * generalErrorSubHandling	: WaitForHandle "->" FinalNode
//	 * generalErrorSubFixing	: WaitForFix (FinalNode)
//	 * ================================================
//	 */
//	// SEND BY |WaitForFix|
//	exampleFSM->handleMessage(EVT_ERROR_HANDLED, 0);
//	std::cout << std::string(60,'-') << '\n';
//	usleep(1000 * 1000);
//
//
//
//	/*
//	 * EVENT: EVT_STOP
//	 * ================================================
//	 * state					: GeneralError -> Idle
//	 * subStateRunning			: ReadyWaiting
//	 * generalErrorSubHandling	: WaitForHandle (FinalNode)
//	 * generalErrorSubFixing	: WaitForFix (FinalNode)
//	 * ================================================
//	 */
//	std::cout << "#TRIGGER# EVT_STOP\n";
//	exampleFSM->handleMessage(EVT_STOP, 0);
//	usleep(1000 * 1000);
//
//	std::cout << std::string(60,'=') << '\n';
//	printf("DONE!\n");
//	delete exampleFSM;
//
//	return 0;
//}
