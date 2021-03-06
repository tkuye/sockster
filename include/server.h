#ifndef SERVER_H
#define SERVER_H
#include "event2/event.h"
#include "event2/listener.h"
#include "event2/visibility.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#define PORT 2601

extern struct sockaddr_in new_sin;

extern struct evconnlistener * listener;

extern evutil_socket_t server_socket;

extern struct event_base * event_loop; 

void event_callback(evutil_socket_t sock, short events, void * c);
// Function used for the creation of a socket. 
evutil_socket_t generate_socket();
// Fucntion responsible for the main server creation code
void create_server();


struct event * create_read(evutil_socket_t socket, struct event_base * events,  void * ptr);


void reader_buf(struct bufferevent *bev, void *ctx);
// Callback function for new connections to our listener 
static void socket_callback(struct evconnlistener * listener, evutil_socket_t sock, struct sockaddr * addr, int socklen, void * cc); //
void bufferevent_callback(struct bufferevent *bev, short flags, void *ctx);

#endif