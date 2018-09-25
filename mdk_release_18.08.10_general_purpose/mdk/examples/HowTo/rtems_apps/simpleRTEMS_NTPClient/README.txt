simpleRTEMS_NTPClient

Supported Platform
==================
Myriad2 - This example works on Myriad2: ma2150, ma2450 silicon

Overview
==========
The example initializes the network stack in POSIX_Init, prints out networks statistics and finally creates and starts a thread. Part of this initialization involves
getting an IP address from the network DHCP and obtaining DNS servers and default gateway.
The thread will resolve an address and create a UDP connection against a NTP Server. After issuing the request it will wait for the response. Once the response is
obtained, RTEMS Time of Day is updated so that it can be used as a reference for further queries. Finally the example prints out the time fetched from the server.

Hardware needed
==================
Myriad2 - This software should run on MV182 or MV212 boards.
Ethernet connection - MV182 or MV212 must be connected to a network with a DHCP server available. Supporting upto 100Mbps mode.

Build
==================
Please type "make help" if you want to learn available targets.

!!!Before cross-compiling make sure you do a "make clean"!!!

Myriad2 - To build the project please type:
     - "make clean"
     - "make all MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Host changing
==================
If pool.ntp.org is not reachable, needs a proxy to be reached, or simply you want to change the target site,
build the example with extra HOST variable like this: make all HOST=<reachable ntp target site>
For example, in order to test against time.nist.gov, use make all -j8 HOST=time.nist.gov
When internet connection is through a proxy server use a host name that is accessible in local networking that hosts an NTP server.

Setup
==================
Myriad2 silicon - To run the application:
    - open terminal and type "make start_server"
    - open another terminal and type "make debug MV_SOC_REV={Myriad_version}"

Where {Myriad_version} may be ma2150 or ma2450.
The default Myriad revision in this project is ma2450 so it is not necessary
to specify the MV_SOC_REV in the terminal.

Expected output
==================
UART:
UART: rtems_leon_greth_gbit_driver_setup RTEMS_SUCCESSFUL
UART: dhcpc: gr_eth: inet: 192.168.86.21   mask: 255.255.252.0
UART:              srv: 192.168.85.30     gw: 192.168.85.1
UART: ************ INTERFACE STATISTICS ************
UART: ***** lo0 *****
UART: Address:127.0.0.1       Net mask:255.0.0.0
UART: Flags: Up Loopback Running Multicast
UART: Send queue limit:50   length:0    Dropped:0
UART: ***** gr_eth1 *****
UART: Ethernet Address: 11:11:11:11:11:11
UART: Address:192.168.86.21   Broadcast Address:192.168.87.255  Net mask:255.2
UART: 55.252.0
UART: Flags: Up Broadcast Running Simplex
UART: Send queue limit:50   length:0    Dropped:0
UART:   Rx Interrupts:       28
UART:   Rx Packets:          28
UART:   Length:               0
UART:   Non-octet:            0
UART:   Bad CRC:              0
UART:   Overrun:              0
UART:   Tx Interrupts:        0
UART:   Maximal Frags:        2
UART:   GBIT MAC:             0
UART:
UART: ***************Inet Routes****************
UART: Destination     Gateway/Mask/Hw    Flags     Refs     Use Expire Interfa
UART: ce
UART: default         192.168.85.1       UGS         0        0      0 gr_eth1
UART:
UART: 127.0.0.1       127.0.0.1          UH          0        0      0 lo0
UART: 192.168.84.0    255.255.252.0      U           0        0     13 gr_eth1
UART:
UART: 192.168.85.1    00:1D:AA:AD:1A:38  UHL         1        0   1214 gr_eth1
UART:
UART: 192.168.85.30   B0:83:FE:C2:91:B2  UHL         0        1   1213 gr_eth1
UART:
UART:
UART: Resolving name to IP addresses:
UART:
UART:                 *Host Address 0: 85.91.1.180
UART:
UART:                 *Host Address 1: 193.1.219.116
UART:
UART:                 *Host Address 2: 54.194.18.100
UART:
UART:
UART: SNTP request to 85.91.1.180:  48 bytes sent
UART:
UART: SNTP response from 85.91.1.180 : 48 bytes received
UART:
UART: rtems_clock_set RTEMS_SUCCESSFUL
UART:
UART: Current Time of Day: Tue Feb 10 12:14:56 2015
UART:



User interaction
==================

