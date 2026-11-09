#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>

#include "MQTTpaho/MQTTClient.h"



#define BROKER_ADR        "tcp://192.168.220.1:1883"
static  char * clientId[] = {"QNX_node_1","QNX_node_2"};

#define NO_OF_TOPICS		10
#define NO_OF_ITERATIONS	4
#define STR_BUF_SIZE		100
#define QOS         		1
#define TIMEOUT     		10000L

#define FESTO_1 1

class MqttReceiver{
	int atLeastOneTopicArrived = 0;
	public:
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
		MqttReceiver *recv = (MqttReceiver*) context;
	    printf("Message arrived in class\n");
	    printf("     topic: %s\n", topicName);
	    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
	    MQTTClient_freeMessage(&message);
	    MQTTClient_free(topicName);
	    recv->atLeastOneTopicArrived = 1;
	    return 1;
	}
};



static MQTTClient client;
static MqttReceiver receiver;

static int atLeastOneTopicArrived = 0;

/*
 * Call back functions
 */

void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
}

//int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
//    printf("Message arrived\n");
//    printf("     topic: %s\n", topicName);
//    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
//    MQTTClient_freeMessage(&message);
//    MQTTClient_free(topicName);
//    atLeastOneTopicArrived = 1;
//    return 1;
//}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

/*
 * MQTT interface for one client
 */

int connectClient(const char * brokerAdr, const char * clientId){
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;

	if ((rc = MQTTClient_create(&client, brokerAdr, clientId,
	    MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
	    printf("Failed to create client, return code %d\n", rc);
	    return rc;
	}
//	if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS) {
	if ((rc = MQTTClient_setCallbacks(client, (void*) &receiver, connlost, &receiver.msgarrvd, delivered)) != MQTTCLIENT_SUCCESS) {
	        printf("Failed to set callbacks, return code %d\n", rc);
	        MQTTClient_destroy(&client);
	        return rc;
	}

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to connect, return code %d\n", rc);
		MQTTClient_destroy(&client);
		return rc;
	}
	return rc;
}

int disconnectClient(void){
	int rc;

	if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) {
	    	printf("Failed to disconnect, return code %d\n", rc);
	    	return rc;
	}
	MQTTClient_destroy(&client);
	printf("Client disconnected and destroyed.\n");
	return rc;
}

int subscribeTopic(char * topic, int qos){
	int rc;
	if ((rc = MQTTClient_subscribe(client, topic, qos)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to subscribe topic %s, return code %d\n", topic, rc);
		return rc;
	}
	printf("Subscribing to topic %s using QoS%d\n", topic, qos);
	return rc;
}

int unsubscribeTopic(char * topic){
	int rc;
	if ((rc = MQTTClient_unsubscribe(client, topic)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to unsubscribe topic %s, return code %d\n", topic, rc);
	    return rc;
	}
	printf("Topic %s unsubscribed.\n", topic);
	return rc;
}

int publishMsg(char * clientId, char * topic, char *payload){
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;

	pubmsg.payload = payload;
	pubmsg.payloadlen = strlen(payload);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	int rc;

	if ((rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to publish topic %s (value = %s), return code %d\n", topic, payload, rc);
		return rc;
	};
	printf("Waiting for up to %d seconds for publication of %s\n"
	       "on topic %s for client with ClientID: %s\n",
	       (int)(TIMEOUT/1000), payload, topic, clientId);
	if ((rc = MQTTClient_waitForCompletion(client, token, TIMEOUT)) != MQTTCLIENT_SUCCESS) {
		printf("Failed to publish topic %s (value = %s) - not completed, return code %d\n", topic, payload, rc);
		return rc;
	}
	printf("Message with delivery token %d delivered\n", token);
	return rc;
}

/*
 * Simple test
 */

void genTopic(char * topicStrBuf, int bufSize, int vmNo, int topicNo) {
	snprintf(topicStrBuf, bufSize, "VM_%d/topic_%d", vmNo, topicNo);
}

void genTopicValue(char * valueStrBuf, int bufSize, int vmNo, int topicNo, int iteration) {
	snprintf(valueStrBuf, bufSize, "from_VM_%d topic no %d iteration %d.", vmNo, topicNo, iteration);
}

int main(int argc, char* argv[]) {
	// read command line parameters
	int virtualMachineNo = 0;
	if(!FESTO_1){
		virtualMachineNo = 1;
	}


//	if (argc < 2) {
//        printf("Usage: %s VM1 | VM2 \n", argv[0]);
//        return EXIT_FAILURE;
//    }
//    printf("Use configuration for %s\n",argv[0]);
//    if (strcmp(argv[1], "VM1") == 0) {
//    	virtualMachineNo = 0;
//    }
//    else if (strcmp(argv[1], "VM2") == 0) {
//    	virtualMachineNo = 1;
//    }
//    else {
//        printf("Usage: %s VM1 | VM2 \n", argv[0]);
//        return EXIT_FAILURE;
//    }



    // setup MQTT client in synchronous mode
    if (connectClient(BROKER_ADR, clientId[virtualMachineNo]) != MQTTCLIENT_SUCCESS) {
    	return EXIT_FAILURE;
    }

    // subsribe topics from other VM
    for (int i = 0; i < NO_OF_TOPICS; i++){
    	char topic[STR_BUF_SIZE];
    	genTopic(topic, sizeof(topic), (0 == virtualMachineNo)?2:1, i);
    	if (subscribeTopic(topic,QOS) != MQTTCLIENT_SUCCESS){
    		for (int j = 0; j < i; j++){
    			genTopic(topic, sizeof(topic), (0 == virtualMachineNo)?2:1, j);
    			unsubscribeTopic(topic);
    		}
    		disconnectClient();
    	    return EXIT_FAILURE;
    	}
    }

    // publish topics
    for (int it = 0; it < NO_OF_ITERATIONS; it++){
    	for (int top = 0; top < NO_OF_TOPICS; top++){
    		char topic[STR_BUF_SIZE];
    		char value[STR_BUF_SIZE];
			genTopic(topic, sizeof(topic), 1 + virtualMachineNo, top);
			genTopicValue(value, sizeof(value), 1 + virtualMachineNo, top, it);
			publishMsg(clientId[virtualMachineNo], topic, value);
			// wait for first msg of other VM
			while(atLeastOneTopicArrived == 0); // very dirty
    	}
    	sleep(it);
    }
    sleep(30);
    // unsubsribe topics from other VM
    for (int i = 0; i < NO_OF_TOPICS; i++){
    	char topic[STR_BUF_SIZE];
     	genTopic(topic, sizeof(topic), (0 == virtualMachineNo)?2:1, i);
     	if (unsubscribeTopic(topic) != MQTTCLIENT_SUCCESS){
     		disconnectClient();
     		return EXIT_FAILURE;
     	}
    }
    // disconnect client
    if (disconnectClient() != MQTTCLIENT_SUCCESS) {
       	return EXIT_FAILURE;
    }
}

