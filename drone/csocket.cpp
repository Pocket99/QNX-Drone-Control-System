// standard needed header files.
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "csocket.h"
using namespace std;

csocket::csocket(){
    backlog=10;/* set the default number of pending connections queue to 10 */
    sockfd = -1;
    send_recv_sockfd=-1; // mark the send_recv_sockfd as invalid
    open();
}

csocket::~csocket(){
    close_session();
    c_close();
}

int csocket::open(){
    // open a TCP socket and save the handler in sockfd
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return 0;
}

int csocket::close_session(){
    // check that is session socket created or not
    // close the session socket if the socket descriptor is valid
    send_recv_sockfd=-1;
    return 0;

}

int csocket::c_close(){ // close the accepted connections

    // chech that is socket created or not

    // close the socket if the socket descriptor is valid
    close(sockfd);
    sockfd=-1;
    return 0;

}

int csocket::c_bind(unsigned short int port){

    // the return value of function

    int returnval;

    // set the port number

    this->port=port;

    /* host byte order */

    server_addr.sin_family = AF_INET;

    /* short, network byte order */

    server_addr.sin_port = htons(port);

    server_addr.sin_addr.s_addr = INADDR_ANY; // local host

    /* zero the rest of the struct */

    memset(&(server_addr.sin_zero), 0, 8);

    // to make sure it does not block when socket related functions are called

    int flags;

    flags=fcntl(sockfd,F_GETFL,0);

    if(flags==-1){

        cout<<"Error in fcntl() call."<<endl;

    }else{

        if(fcntl(sockfd,F_SETFL,flags | O_NONBLOCK)==-1)

        cout<<"Error in fcntl() call."<<endl;

    }

    // To make sure the server does not block the other threads to use this port

    int option=1;

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(int));

    // use addr to bind the socket
    socklen_t addrlen = sizeof(server_addr);
    returnval = bind(sockfd,(struct sockaddr*)&server_addr, addrlen);
    if(returnval < 0){
        cout<<"bind fail"<<endl;
    }
    return returnval;
}

int csocket::c_connect(unsigned short int HostPort, char * HostIP){
    // the return value of function
    int returnval;
    // set the port number
    this->port=port;
    /* host byte order */
    server_addr.sin_family = AF_INET;
    /* short, network byte order */
    server_addr.sin_port = htons(HostPort);
    server_addr.sin_addr.s_addr = inet_addr(HostIP);
    /* zero the rest of the struct */
    memset(&(server_addr.sin_zero), 0, 8);
    // connect to the server
    returnval = connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(returnval < 0){
        cout<<"not connect"<<endl;
    }
    send_recv_sockfd=sockfd; // set the send_recv_sockfd equal to sockfd

    return returnval;

}

int csocket::c_listen(){

    // the return value of function

    int returnval;

    // listen to the port
    returnval = listen(sockfd,backlog);
    return returnval;

}

int csocket::c_accept(){

// accept the connection from client
    socklen_t c = sizeof(client_addr);
    send_recv_sockfd = accept(sockfd,(sockaddr*)&client_addr, &c);
    return send_recv_sockfd;

}

int csocket::c_send(char * buff, int len){

    int returnvalue;

    // send data
    returnvalue = send(sockfd , buff, len , 0);
	if(returnvalue < 0){
        cout<<"send fail"<<endl;
    }else{
    	cout<<"send success"<<endl;
    }
    return returnvalue;

}

int csocket::c_send_to_client(char * buff, int len){
	 int returnvalue;

    // send data
    returnvalue = send(send_recv_sockfd , buff, len , 0);
	if(returnvalue < 0){
        cout<<"send fail"<<endl;
    }else{
    	cout<<"send success"<<endl;
    }
    return returnvalue;
}

int csocket::c_receive(char * buff, int len){

    int returnvalue;

    // receive
    returnvalue = recv(sockfd, buff, len, 0);
    if(returnvalue < 0){
        cout<<"receive fail"<<endl;
    }
    return returnvalue;

}

int csocket::c_receive_from_server(char * buff, int len){
 	int returnvalue;

    // receive
    returnvalue = recv(send_recv_sockfd , buff, len, 0);
    if(returnvalue < 0){
        cout<<"receive fail"<<endl;
    }
    return returnvalue;
}