#include <event2/event.h>
#include <event2/util.h>
#include "server.h"
#include <event2/visibility.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <event2/listener.h>
#include <stdlib.h>
#include <arpa/inet.h>


evutil_socket_t generate_socket(){
    ev_socklen_t sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        exit(1);
    }
    evutil_make_listen_socket_reuseable(sock);
    return sock;
}


void create_server(){
// Socket for listening

struct event * reader; 

evutil_socket_t server_socket;
struct sockaddr_in new_sin; 
struct event_base * event_loop; 
struct evconnlistener * listener; 

server_socket = generate_socket();

memset(&new_sin,0, sizeof new_sin);
new_sin.sin_family = AF_INET;
new_sin.sin_port = htons(PORT); 
new_sin.sin_addr.s_addr = inet_addr("127.0.0.1");

event_loop = event_base_new();

if (!event_loop){
    printf("Cannot create event loop\n");
    exit(1);
}

listener = evconnlistener_new_bind(event_loop,
socket_callback, // callback function
NULL, // Pointer to pass later
LEV_OPT_THREADSAFE|LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, // thread safe 
50, // back log,may change later
(struct sockaddr * )&new_sin, 
sizeof new_sin);

if (!listener){
    fprintf(stderr, "Could not create listener"); 
    exit(1);
}

reader = event_new(
    event_loop,
    server_socket,
    EV_READ, 
    event_callback, 
    NULL
);

if (!reader){
    printf("Cannot create event");
}
int err = event_add(reader, NULL); 

if (err == -1){
   fprintf(stderr, "cannot add event"); 
}
printf("Server Socket: %d\n", server_socket);

for(;;)
event_base_loop(event_loop, EVLOOP_NONBLOCK);


printf("Using Libevent with backend method %s.\n",
        event_base_get_method(event_loop));

/*if ( == -1){
    fprintf(stderr, "cannot create loop");
    exit(1); 
}*/

}

void socket_callback(struct evconnlistener * listener, evutil_socket_t sock, struct sockaddr * addr, int socklen, void * cc){
    // Socket call back will include the new socket we added. 
    printf("New Connection Socket %d\n", sock);
}


void event_callback(evutil_socket_t sock, short events, void * c)
{
printf("Helloooo!\n"); 
}