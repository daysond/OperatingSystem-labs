# SEP400 - Lab 5: Server with Multiple Clients and a Receive Thread with Mutexing

The recording for lab 5 can be found at [Lab5.mp4](https://seneca-my.sharepoint.com/:v:/g/personal/miguel_watler_senecacollege_ca/ERWyPjsd-0tOrD5Cp5qRAGcB_YDnrBc6ymARwoswuz3WLg?e=etaDir).

## Due: Sunday, February 26, 2023

In this lab you will create a server (**server.cpp**) that communicates with three clients using a receive thread for each. The client code has been given to you along with the Makefile, a start-up script, and a stop script in case you have to manually stop all the clients. You can retrieve these from the following links:

[client.cpp](client.cpp),

[Makefile](Makefile),

[startClient.sh](startClient.sh),

[stopClient.sh](stopClient.sh).


Your server will use non-blocking stream sockets in the internet domain. Your socket will bind to the **localhost (127.0.0.1)** and the port number will be specified from the command line. For instance, if you want to use port 1153, you would start your server and clients with:
```
$ ./server 1153
$ ./startClient.sh 1153
```

Be sure to start your server first. You will probably want to start your clients in another window.

**startClient.sh** will start three clients with a 1 second delay between each.

Once your socket is bound, you will listen for connections. Be sure to distinguish between the master file descriptor and the connection file descriptors for each of the clients. You will have one master file descriptor for listening and accepting, but three connection file descriptors for each of the three clients.

Once a client connects, you will start off a **receive thread** to service all socket reads from this client. In this way you avoid using the `select()` function. Be sure to pass through the connection file descriptor for that client to the thread as a parameter. This means your socket reads and writes are no longer synchronized as before. They are asynchronous. You will perform socket writes from your `main()` function and socket reads from your **receive threads**.

The **receive thread** will change the options of its connection socket to set a read timeout of 5 seconds. Documentation on how to do this can be found at:
[setsockopt - Linux man page](https://linux.die.net/man/2/setsockopt),

Chapter 61 page 624 of the text [Advanced Programming in the UNIX Environment](https://ict.senecacollege.ca/~miguel.watler/courses/unx511/Advanced%20Programming%20in%20the%20UNIX%20Environment%203rd%20Edition.pdf), 

and the article [Linux: is there a read or recv from socket with timeout?](https://stackoverflow.com/questions/2876024/linux-is-there-a-read-or-recv-from-socket-with-timeout).

Since communications are now asynchronous, the server does not have to instruct the client to send data. As soon as the client connects to the server, the client will start sending data (see client.cpp).

When a client sends text to the server, the server will read the data in its **receive thread** and push it into a message queue. The message queue could be of type:
**queue< string > message**;

The `main()` function, in its infinite while-loop, will test for new connections with a non-blocking call to `accept()`, and write the contents of the message queue to the screen if there is anything in the message queue. If the queue is empty, `main()` will sleep for 1 second and check the queue size again. Be sure to pop the message off the message queue once you have written it to the screen.

The infinite while loop (conditioned on `is_running`) in `main()` should look something like this:

- Have we accepted all clients?
    - If not, call `accept()`. Since the master socket file is non-blocking, `accept()` will not halt the code if no client wishes to connect.
    - If a client wishes to connect, create a thread for that client, passing through the connection file descriptor and increment the number of clients.
-   Print out to the screen anything in the message queue. Be sure to mutex this message queue since it is used in the receive thread as well.
-   Sleep for one second.

The receive thread should look like this (the same code can be used for all three clients):

-   Extract the connection file descriptor from the argument passed to the receive thread.
-   Use `setsockopt()` to set the read timeout to 5 seconds.
-   Enter infinite while loop conditioned on the variable `is_running`.
    -   If there is something on the read, add it to the message queue. Be sure to mutex this message queue since it is used in main() as well.
    -   Note that read should block for a maximum of five seconds.

The communications stop when a **ctrl-C** is issued to the server. The signal handler for the server will set `is_running` to `false` causing the infinite while-loops in the **receive thread** and in `main()` to end. The server in its `main()` function will then send **"Quit"** to each client, close all connections, and exit. The clients will interpret **"Quit"** to mean it is time to finish and they will shutdown as well. Be sure to join the **receive threads** to `main()` before `main()` exits and be sure to close all connection file descriptors, as well as the master file descriptor.

## Questions
1.  What is the difference between synchronous and asynchronous communication?

Synchronous communication waits for the result before executing the next line of code. It's blocking, can be inefficient if there's delay.

Asynchronous does not wait for the result and will go ahead and execute the rest of the code. It's non-blocking.  In socket communication, this can be achieved using multiple threads, select() or poll(). In general, callbacks or event-driven programming can be used.

2.  Do you prefer socket reads in a receive thread or do you prefer both socket read and write to be in the main() function? Why?

It depends. If the program is rather simple, one server one client, just read and write, I prefer doing both read and write in the main function. This way, less system resouces will be used and the code will be simpler. If the program needs to allow connections of multiple clients, it's better to have seperate threads for read and write. This way, our program will be more responsive. However, this adds complexity to the code.