// /**
// * AdcDemo.cpp
// *
// *  Created on: 15.11.2021
// *      Author: Bjoern Dittmann
// *
// * Demonstriert den Zugriff auf den simulierten ADC im ESE Praktikum.
// * Dieses Beispiel nutzt eine eigene ISR (InterruptAttach), es ist aber auch
// * möglich die selbe Funktionalität mit InterruptAttachEvent zu realisieren.
// */

// #include <stdio.h>
// #include <errno.h>
// #include <sys/neutrino.h>
// #include <sys/procmgr.h>
// #include <iostream>
// #include <thread>
// #include <chrono>

// #include "simqnxgpioapi.h"
// #include "simqnxirqapi.h"


// /* Code configuration */
// #define DEMO true			//true for demo time, false to spin forever.
// #define DEMO_DURATION 3		//Duration of demo time in Timetracking.


// /* Interrupt numbers  (spruh73l.pdf S.465 ff.) */
// #define INTER_ADC 16


// /* GPIO port addresses (spruh73l.pdf S.177 ff.) */
// #define GPIO_PORT0 0x44E07000
// #define GPIO_PORT1 0x4804C000
// #define GPIO_PORT2 0x481AC000
// #define ADC_BASE 0x44E0D000


// /* GPIO port registers length */
// #define GPIO_REGISTER_LENGHT 0x1000
// #define ADC_LENGTH 0x2000


// /* GPIO register offsets (spruh73l.pdf S.4877) */
// #define GPIO_DATAIN 0x138
// #define GPIO_SETDATAOUT 0x194


// /* TSC_ADC register offsets (spruh73l.pdf S.1747) */
// #define ADC_IRQ_ENABLE_SET 0x2c
// #define ADC_IRQ_ENABLE_CLR 0x30
// #define ADC_IRQ_STATUS 0x28
// #define ADC_CTRL 0x40
// #define ADC_DATA 0x100


// /* ADC irq pin mask */
// #define ADC_IRQ_PIN_MASK 0x2


// /* Actuators pin mapping (Aufgabenbeschreibung o. Schaltplan) */
// #define MOTOR_RIGHT_PIN 12
// #define MOTOR_SLOW_PIN 14


// /* My pulse codes */
// #define PULSE_STOP_THREAD _PULSE_CODE_MINAVAIL + 1
// #define PULSE_ADC_SAMLING_DONE _PULSE_CODE_MINAVAIL + 2


// /* Helper macros */
// #define BIT_MASK(x) (0x1 << (x))


// /* Variables */
// sigevent event;
// int interruptID;
// uintptr_t adcBaseAddr;
// bool receivingRunning = false;


// /* Prototypes */
// const struct sigevent* adcISR(void* arg, int id);

// int main(int argc, char** args);
// void receivingRoutine(int);

// void adc_enable_interrupt(void);
// void adc_disable_interrupt(void);
// void adc_clear_interrupt(void);
// void adc_ctrl_start_sample(void);
// uint32_t adc_read_sample_data(void);


// /* Implementations */
// using namespace std;

// const struct sigevent* adcISR(void* arg, int id) {
// 	sigevent* event = (sigevent*) arg;

// 	uint32_t value = adc_read_sample_data();

// 	adc_clear_interrupt();
// 	InterruptUnmask(INTER_ADC, interruptID);

// 	event->sigev_value.sival_int = value;
// 	return event;
// }


// int main(int argc, char** args) {

// 	/* ### Setup ### */
// 	ThreadCtl(_NTO_TCTL_IO, 0);	//Request IO privileges for process.

// 	// Request interrupt and IO abilities.
// 	int procmgr_status = procmgr_ability(
// 		0,
// 		PROCMGR_ADN_ROOT 	| PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
// 		PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
// 		PROCMGR_ADN_ROOT 	| PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
// 		PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
// 		PROCMGR_AID_EOL
// 	);
// 	if (procmgr_status != EOK) {
// 		perror("Requested abilities failed or denied!");
// 		exit(EXIT_FAILURE);
// 	}

// 	InterruptEnable();			//Enables interrupts.

// 	adcBaseAddr = mmap_device_io(ADC_LENGTH, ADC_BASE); //Setup global ADC base address.



// 	/* ### Create channel ### */
// 	int chanID = ChannelCreate(0); //Create channel to receive interrupt pulse messages.
// 	if (chanID < 0) {
// 		perror("Could not create a channel!\n");
// 		return EXIT_FAILURE;
// 	}

// 	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); //Connect to channel.
// 	if (conID < 0) {
// 		perror("Could not connect to channel!");
// 		return EXIT_FAILURE;
// 	}



// 	/* ### Start motor slow for demo ### */
// 	uintptr_t port1BaseAddr = mmap_device_io(GPIO_REGISTER_LENGHT, GPIO_PORT1);
// 	out32((uintptr_t) port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_RIGHT_PIN));
// 	out32((uintptr_t) port1BaseAddr + GPIO_SETDATAOUT, BIT_MASK(MOTOR_SLOW_PIN));



// 	/* ### Setup interrupt ### */
// 	SIGEV_PULSE_INIT(&event, conID, SIGEV_PULSE_PRIO_INHERIT, PULSE_ADC_SAMLING_DONE, 0);

// 	/* Attach ISR  */
// 	interruptID = InterruptAttach(INTER_ADC, adcISR, &event, sizeof(sigevent), 0);
// 	if (interruptID == -1) {
// 		perror("InterruptAttach failed!");
// 		exit(EXIT_FAILURE);
// 	}



// 	/* ### Start thread for handling interrupt messages. */
// 	thread receivingThread = thread(&receivingRoutine, chanID);



// 	/* ### Start sampling ### */
// 	adc_clear_interrupt();		//clear interrupt (just in case)
// 	adc_enable_interrupt();		//enable interrupt
// 	adc_ctrl_start_sample();	//start sampling


// 	/* ### Waiting for demo. ### */
// 	if(DEMO){
// 		printf("Demo time for %d Timetracking...\n", DEMO_DURATION);
// 		this_thread::sleep_for(chrono::Timetracking(DEMO_DURATION));	//main thread sleep.
// 		printf("Stopping in...\n");
// 		printf("3\n");
// 		this_thread::sleep_for(chrono::seconds(1));
// 		printf("2\n");
// 		this_thread::sleep_for(chrono::seconds(1));
// 		printf("1\n");
// 		this_thread::sleep_for(chrono::seconds(1));
// 		printf("NOW!\n");
// 	} else {
// 		while(true){
// 			this_thread::sleep_for(chrono::hours(999999));
// 		}
// 	}


// 	/* ### Cleaning up ### */

// 	// Detach interrupts.
// 	adc_clear_interrupt();
// 	adc_disable_interrupt();
// 	int intr_detach_status = InterruptDetach(interruptID);
// 	if(intr_detach_status != EOK){
// 		perror("Detaching interrupt failed!");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Stop receiving thread.
// 	MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); //using prio of calling thread.
// 	receivingThread.join();

// 	// Close channel
// 	int detach_status = ConnectDetach(conID);
// 	if(detach_status != EOK){
// 		perror("Detaching channel failed!");
// 		exit(EXIT_FAILURE);
// 	}

// 	int destroy_status = ChannelDestroy(chanID);
// 	if(destroy_status != EOK){
// 		perror("Destroying channel failed!");
// 		exit(EXIT_FAILURE);
// 	}

// 	return EXIT_SUCCESS;
// }



// void receivingRoutine(int channelID) {

// 	ThreadCtl( _NTO_TCTL_IO, 0);	//Request IO privileges for this thread.

// 	_pulse msg;
// 	receivingRunning = true;

// 	printf("Message thread started.\n");

// 	while (receivingRunning) {
// 		int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);

// 		if (recvid < 0) {
// 			perror("MsgReceivePulse failed!");
// 			exit(EXIT_FAILURE);
// 		}

// 		if (recvid == 0) {	//pulse received.

// 			//Stop thread while it blocks.
// 			if (msg.code == PULSE_STOP_THREAD) {
// 				printf("Thread kill code received!\n");
// 				receivingRunning = false;
// 				continue;
// 			}

// 			//ADC interrupt value.
// 			if (msg.code == PULSE_ADC_SAMLING_DONE) {
// 				printf("Value from adc with value %d!\n", msg.value);
// 				adc_ctrl_start_sample();	//Start next sampling.
// 			}

// 			// Do not ignore OS pulses!
// 		}
// 	}

// 	printf("Message thread stops...\n");
// }


// void adc_enable_interrupt(void){
// 	out32((uintptr_t) adcBaseAddr + ADC_IRQ_ENABLE_SET, ADC_IRQ_PIN_MASK);
// }


// void adc_disable_interrupt(void){
// 	out32((uintptr_t) adcBaseAddr + ADC_IRQ_ENABLE_CLR, ADC_IRQ_PIN_MASK);
// }


// void adc_clear_interrupt(void){
// 	out32((uintptr_t) adcBaseAddr + ADC_IRQ_STATUS, ADC_IRQ_PIN_MASK);
// }


// void adc_ctrl_start_sample(void){
// 	out32((uintptr_t) adcBaseAddr + ADC_CTRL, 0x1);
// }

// uint32_t adc_read_sample_data(void){
// 	return in32((uintptr_t) adcBaseAddr + ADC_DATA);
// }
