#ifndef CQSTREAMSRV_H_
#define CQSTREAMSRV_H_
#include <pthread.h>
#include <iostream>
#include "csocket.h"
#include "cjoystick.h"
class cqstreamsrv{
	private:
		pthread_t thread_id;
		pthread_mutex_t mutex;
		int stop_req;
		union Data{
			double snd_data[8];
			char temp_data[8];
		}data;
		double local_data[8];
		cjoystick* cj;
		bool is_data_copied2buff, is_data_sent;
		friend void * thread_stud_srv(void* arg);
	public:
		int port;
		int sample_time;
		csocket s;
		cqstreamsrv(int port,int sample_time);
		~cqstreamsrv();
		int new_data_is_sent();
		void send_data_array(double data[]);
		void send_data(double d0,double d1,double d2,double d3,double d4,double d5,double d6,double d7);
		void stop();
		void start();
		void server();
}; 
#endif /*CQSTREAMSRV_H_*/