# SEP400 - Lab 2: I/O Control - Network Interface

## Due: Friday, February 3, 2023

In this lab you will make use of I/O control (ioctl) and sockets to get information from a network interface. You will use ioctl's to retrieve an interface's MAC address, IP address, netmask, and broadcast address.

Your program will ask you for the name of a network interface. To see the network interfaces on your machine type:

    ifconfig

A user interface will give the user the following selections:

    Choose from the following:
    1. Hardware address
    2. IP address
    3. Network mask
    4. Broadcast address
    0. Exit

A program has already been created that prompts the user for the network interface name, that displays the above user interface, and that already retrieves a network interface's MAC address - [**etherCtrl.cpp**](etherCtrl.cpp). You need to complete this program to retrieve:

**1. The IP address (case 2),**

**2. The Network mask (case 3),**

**3. The Broadcast address (case 4).**

<br/>

Please provide a Makefile for this program.

For documentation on low-level access to Linux network devices, see [netdevice](https://man7.org/linux/man-pages/man7/netdevice.7.html).

For documentation on how to get the IP address of a network interface, see [Get IP Address in Linux using C program](https://www.includehelp.com/c-programs/get-ip-address-in-linux.aspx), from includehelp.com.

<br/>

Test your program with existing network interfaces on your machine and also with the loopback interface lo. Compare the results of your program with those given by **ifconfig**. They should be the same.
