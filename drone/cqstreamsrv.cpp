// standard needed header files.
#include <string.h>
#include <iostream.h>
#include "csocket.h"
#include "ctimer.h"
#include "cqstreamsrv.h"
using namespace std;
cqstreamsrv::cqstreamsrv(int port,int sample_time){
	this->port=port;
	this->sample_time=sample_time;
	thread_id=NULL;
	cj = new cjoystick();
	pthread_mutex_init(&mutex,NULL);
	start();
}
cqstreamsrv::~cqstreamsrv(){
	stop();
	pthread_mutex_destroy(&mutex);
}
void cqstreamsrv::start(){
	int i;
	stop_req=0;
	is_data_copied2buff=false;
	is_data_sent=false;
	for(i=0;i<8;i++) data.snd_data[i]=0;
	if(pthread_create(&thread_id,NULL,thread_stud_srv,(void *) this)!=EOK)
	thread_id=NULL;
}
void cqstreamsrv::stop(){
	stop_req=1;
	if(thread_id != NULL)
	pthread_join(thread_id,NULL);
	thread_id=NULL;
}
void cqstreamsrv::send_data_array(double data[]){
	pthread_mutex_lock(&mutex);
	//copy data to the buffer
	int i;
	for(i=0;i<8;i++){
		local_data[i] = data[i];
	}
	is_data_copied2buff=false;
	pthread_mutex_unlock(&mutex);
}
void cqstreamsrv::send_data(double d0,double d1,double d2,double d3,double d4,double d5,double d6,double d7){
	pthread_mutex_lock(&mutex);
	//copy data to the buffer
	data.snd_data[0] = d0;
	data.snd_data[1] = d1;
	data.snd_data[2] = d2;
	data.snd_data[3] = d3;
	data.snd_data[4] = d4;
	data.snd_data[5] = d5;
	data.snd_data[6] = d6;
	data.snd_data[7] = d7;
	is_data_copied2buff=false;
	pthread_mutex_unlock(&mutex);
}
int cqstreamsrv::new_data_is_sent(){
	if(is_data_sent && is_data_copied2buff)
	return 1;
	else
	return 0;
}
void * thread_stud_srv(void* arg){
	cqstreamsrv& qstream = *(cqstreamsrv *) arg;
	qstream.server();
	return 0;
}
void cqstreamsrv::server(){
	// initiate the timer
	CTimer timer(1,sample_time);
	while( stop_req==0){
		// initialize the socket and bind to the port
		int retval = s.c_bind(port);
		if(retval < 0){
			std::cout<<"bind fail:"<<retval<<std::endl;
		}else{
			std::cout<<"bind success:"<<retval<<std::endl;
			break;
		}
		
	}
	while( stop_req==0){
		// start listening to the port
		int retval = s.c_listen();
		if(retval < 0){
			std::cout<<"listen fail:"<<retval<<std::endl;
		}else{
			std::cout<<"listen success:"<<retval<<std::endl;
			break;
		}
		
	}
	
	while( stop_req==0){
		while( stop_req==0){
			// accepts the client request
			sleep(5);
			int retval = s.c_accept();
			if(retval < 0){
				std::cout<<"accept fail:"<<retval<<std::endl;
			}else{
				std::cout<<"accept success:"<<retval<<std::endl;
				break;
			}
		}
		while( stop_req==0){
			std::cout<<"sending:"<<std::endl;
			pthread_mutex_lock(& mutex);
			//copy snd_data to the local buffer
			send_data_array(data.snd_data);
			is_data_copied2buff=true;
			is_data_sent=false;
			pthread_mutex_unlock(& mutex);
			
			//send local buffers data
			pthread_mutex_lock(& mutex);
			int size = sizeof(local_data);
			cout<<cj->get_status_msg()<<endl;
			if(cj->is_status_ok()){
				cj->print_device_info();
				cout<<cj->get_status_msg()<<endl;
				cout<<cj->getX()<<endl;
				cout<<cj->getY()<<endl;
				cout<<cj->getZ()<<endl;
				cout<<cj->getRZ()<<endl;
				send_data(cj->getX(),cj->getY(),cj->getZ(),cj->getRZ(),0,0,0,0);
			}
			s.c_send_to_client(data.temp_data, size);
			is_data_sent=true;
			pthread_mutex_unlock(& mutex);
			// wait for timer
			timer.wait();
		}
	}
	// close the socket
	s.close_session();
}