//===========================================================================
// standard needed header files.
#include <string.h>
#include <iostream.h>
#include "csocket.h"
#include "ctimer.h"
#include "cqstreamclient.h"
#define BUFLEN 1000

using namespace std;

CQStreamClient::CQStreamClient(char * hostaddress, int port, int sample_time,int ch_size)
{
	this->port = port;
	this->sample_time = sample_time;
	this->ch_size = ch_size;
	strcpy(this->hostaddress, hostaddress);
	thread_id = NULL;
	pthread_mutex_init(&mutex,NULL);
	start();
	
	if(thread_id != NULL)
	pthread_join(thread_id, NULL);
}

CQStreamClient::~CQStreamClient()
{
	stop();
	pthread_mutex_destroy(&mutex);
}

/* Starts thread which runs the client. */
void CQStreamClient::start()
{
	int i;
	stop_req = 0;
	
	/* Clear the rcv_Data array for use. */
	for(i=0; i<ch_size; i++)
	{
		pthread_mutex_lock(&mutex);
		data.rcv_data[i] = 0.0;
		pthread_mutex_unlock(&mutex);
	}
		
	/* Creates thread*/
	if(pthread_create(&thread_id, NULL, thread_stud, (void *) this) != EOK)
		thread_id = NULL;
	
}

void CQStreamClient::stop()
{
	stop_req = 1;
	if(thread_id != NULL)
	pthread_join(thread_id, NULL);
	thread_id = NULL;
}

void CQStreamClient::get_data_array(Data data)
{
	pthread_mutex_lock(&mutex);
	// copy the data from rcv_data to the output buffer
	
	cout << "Gyro X-axis: " << data.rcv_data[0] << endl;
	cout << "Gyro Y-axis: " << data.rcv_data[1] << endl;
	cout << "Gyro Z-axis: " << data.rcv_data[2] << endl;
	cout << "Accel X-axis: " << data.rcv_data[3] << endl;
	cout << "Accel Y-axis: " << data.rcv_data[4] << endl;
	cout << "Accel Z-axis: " << data.rcv_data[5] << endl;
	cout << "Magnt X-axis: " << data.rcv_data[6] << endl;
	cout << "Magnt Y-axis: " << data.rcv_data[7] << endl;
	cout << "Magnt Z-axis: " << data.rcv_data[8] << endl;
	cout << "Battery voltage: " << data.rcv_data[9] << endl;
	cout << "Sonar: " << data.rcv_data[10] << endl;
	cout << "Reserved: " << data.rcv_data[11] << endl;
	
	pthread_mutex_unlock(&mutex);
}

double CQStreamClient::get_data(int i)
{
	double value;
	
	pthread_mutex_lock(&mutex);
	// return the data from rcv_data
	value = data.rcv_data[i];
	pthread_mutex_unlock(&mutex);
	
	return value;
}

void * thread_stud(void* arg)
{
	CQStreamClient& qstream = *(CQStreamClient *) arg;
	qstream.client();
	return 0;
}

void CQStreamClient::client()
{
	int rcv_data_size = 0, timeoutcnt = 0;
	int retval = 0;
	
	// initiate the timer (should probably 5ms)
	CTimer timer(1, 50);
	
	while(stop_req == 0)
	{
		cout << "Port: " << port <<  "  ip:  " << hostaddress << endl;
		
		// initialize the socket and connect to server
		retval = readSensorSocket.c_connect(port, hostaddress);
	
		if(retval != 0)
		{
			cout << "ERROR: Cannot connect to server. Retval: " << retval << " Errno: " << errno << endl;
			return;
		}
		
		cout << "Connected to the server."<< retval << endl;
		timeoutcnt = 0;
		
		while(stop_req == 0)
		{	
			// receiving the data			
			int len;
			
			/* Store byte array into union, can read it as double 12bytes */
			pthread_mutex_lock(&mutex);
			len = sizeof(data.buffer);
			rcv_data_size = readSensorSocket.c_receive(data.buffer, len);		//Here
			cout <<"-----------HERE---------------" << endl;
			pthread_mutex_unlock(&mutex);
			
			// Debug
			cout <<"--------------------------" << endl;
			cout << "Received " << rcv_data_size << " bytes. " << endl;
			
			/* Update timeout counter to see if we need to wait or get values. */
			if(rcv_data_size > 0)
				timeoutcnt = 0;
			else
				timeoutcnt++;
				
			if(rcv_data_size == -1 || timeoutcnt>100)
				break; // there is an error
			
			// Print data to output buffer.
			get_data_array(data);
			
		//wait for the timer
		cout << "Waiting for timer. " << endl;
		timer.wait();
		}
	}
	
	// close the socket
	cout << "Disconnected from the server." << endl;
	readSensorSocket.close_session();
}

	