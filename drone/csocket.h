#ifndef _CSOCKET
#define _CSOCKET
// header files needed for socket and TCP programming
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//=============================================================================
class csocket{
private:
int sockfd; // socket descriptor
int send_recv_sockfd; // socket descriptor of accepted connection
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
public:
unsigned short int port;
int backlog; /* how many pending connections queue will hold */
csocket();
~csocket();
int c_connect(unsigned short int HostPort, char * HostIP);
int c_bind(unsigned short int port);
int c_listen();
int c_accept();
int c_send(char * buff, int len);
int c_send_to_client(char * buff, int len);
int c_receive(char * buff, int len);
int c_receive_from_server(char * buff, int len);

int open();
int c_close();
int close_session();
};
#endif