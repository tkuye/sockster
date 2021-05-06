

#include "event2/event.h"
#include "event2/util.h"
#include "server.h"
#include "event2/visibility.h"
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include "event2/listener.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include <errno.h>

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
-1, // back log,may change later
(struct sockaddr * )&new_sin, 
sizeof new_sin);

if (!listener){
    fprintf(stderr, "Could not create listener"); 
    exit(1);
}

/*
reader = create_read(server_socket, event_loop, NULL); 

if (!reader){
    printf("Cannot create event");
}
int err = event_add(reader, NULL); 

if (err == -1){
   fprintf(stderr, "cannot add event"); 
}*/

printf("Listening on port %d\n", PORT);

for(;;)
event_base_loop(event_loop, EVLOOP_NONBLOCK);


printf("Using Libevent with backend method %s.\n",
        event_base_get_method(event_loop));

/*if ( == -1){
    fprintf(stderr, "cannot create loop");
    exit(1); 
}*/
}

struct event * create_read(evutil_socket_t socket, struct event_base * events,  void * ptr){
    struct event * new_event; 
    new_event = event_new(
    events,
    socket,
    EV_READ, 
    event_callback, 
    ptr);

    return new_event;
}

static void socket_callback(struct evconnlistener * listener, evutil_socket_t sock, struct sockaddr * addr, int socklen, void * cc){
    // Socket call back will include the new socket we added. 
    // create new bufferevent_socket new
    // create bufferevent
    
    struct event_base * base = evconnlistener_get_base(listener);

    printf("SOCKET Callback\n"); 

    // assign socket to it

    struct bufferevent * new_buffer_event = bufferevent_socket_new(base,
    sock,
    BEV_OPT_CLOSE_ON_FREE
    );


    if (!new_buffer_event){
        int err = BEV_EVENT_ERROR; 
        fprintf(stderr, "Error creating bufferevent occured: %s\n", strerror(err)); 
        exit(1);
    }

    if(bufferevent_enable(new_buffer_event, EV_READ | EV_WRITE) == -1){
        perror("bufferevent enable"); //
        exit(0);
    }

    // add the callbacks 
    bufferevent_setcb(new_buffer_event,
    reader_buf,
    NULL,
    bufferevent_callback, 
    NULL
    );

    // add to event listener


}

void bufferevent_callback(struct bufferevent *bev, short flags, void *ctx){
    if (flags & BEV_EVENT_ERROR){
        
        perror("bufferevent_callback");
    }
   printf("bufferevent!\n");
}


void event_callback(evutil_socket_t sock, short events, void * c)
{
printf("Helloooo!\n"); 
}

void reader_buf(struct bufferevent * bev, void * ctx){
    // New message to read
    char message[100]; 
    struct evbuffer * ev = evbuffer_new(); 
    ev = bufferevent_get_input(bev); //


    int buffsize = evbuffer_get_length(ev);
    evbuffer_remove(ev, message, buffsize); 

    printf("The size of the buffer now is %ld message is: ", evbuffer_get_length(ev)); 
    message[buffsize] = '\0';
    printf("%s\n", message);
}