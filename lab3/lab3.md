# SEP400 - Lab 3: Signals

## Due: Friday, February 10, 2023

In this lab you will use signals as a method of communication between a parent process and its children processes. Signals will be used to synchronize the start-up and shutdown of all children processes.

The particular application in mind is a network monitor. The parent will start up children to monitor the network interfaces on a machine. In our case, there are two interfaces: ens33 and lo. There will therefore be two children. One child will monitor ens33 and one child will monitor lo.


The parent process (sysmonExec) will spawn two children (intfMonitor) as follows:


```
$ ./intfMonitor lo
$ ./intfMonitor ens33
```


For the parent, you will have to send the start signal (**SIGUSR1**) to the children, sleep for 30 seconds, then send the stop signal (**SIGUSR2**) to the children. The parent should wait for all children to shutdown before shutting itself down.


For the child, you will have to register signal handlers for start-up (**SIGUSR1**), shutdown (**SIGUSR2**), ctrl-C and ctrl-Z. For ctrl-C and ctrl-Z, your signal handler will simply discard them, meaning your program will not shutdown on ctrl-C nor be put in the background on ctrl-Z. Your child will have to wait until it receives a start signal from the parent before starting. The child's signal handler will handle 4 signals as follows:

If the signal handler receives a **SIGUSR1**, the following message should appear on the screen:

`intfMonitor: starting up`

If the signal handler receives a ctrl-C, the following message should appear on the screen:

`intfMonitor: ctrl-C discarded`

If the signal handler receives a ctrl-Z, the following message should appear on the screen:

`intfMonitor: ctrl-Z discarded`

If the signal handler receives a SIGUSR2, the following message should appear on the screen:

`intfMonitor: shutting down`

If the signal handler receives any other signal, the following message should appear on the screen:

`intfMonitor: undefined signal`

When the child receives the shutdown signal, it should stop processing and exit.
Code has been given to you so you can concentrate on the signals portion only. Simply fill in the parts indicated by **TODO**.

The Makefile can be found at [**Makefile**](Makefile).
The code for the parent can be found at [**sysmonExec.cpp**](sysmonExec.cpp).
The code for the child can be found at [**intfMonitor.cpp**](intfMonitor.cpp).