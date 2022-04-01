#ifndef CQSTREAMCLIENT_H_
#define CQSTREAMCLIENT_H_

#include <pthread.h>
#include "csocket.h"

class CQStreamClient
{
private:
	pthread_t thread_id;	// Handle of running thread. 
	pthread_mutex_t mutex;
	int stop_req;
	
	union Data
	{
		double rcv_data[12];	// Array of sensor data.
		char buffer[sizeof(double)*12];
	}data;
	 
	bool is_data_copied2buff, is_data_sent;
	friend void * thread_stud(void* arg);
	
public:
	int port;				// Port of quadcopter server
	int sample_time;		// Time interval (in msec) for getting data from quadcopter server.
	int ch_size;			// Number of values from sensors.
	char hostaddress[200];	// IP of quadcopter server.
	csocket readSensorSocket;

	CQStreamClient(char * hostaddress, int port, int sample_time, int ch_size);
	~CQStreamClient();
	int new_data_is_sent();
	void get_data_array(Data data);
	double get_data(int i);
	void stop();
	void start();
	void client();
};

#endif /*CQSTREAMCLIENT_H_*/
