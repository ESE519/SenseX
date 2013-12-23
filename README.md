SenseX
======

This project aim's at implementing the RT-Link protocol on the mbed platform. Basically we would like this to be an alternative to the currently implemented B-MAC protocol on the mbed platform.
This repository consists pf 2 implementations namely:-
1)B-MAC Test Implementation
2)RT-LINK Implemnetation

A short Description is as follows:-

1)B-MAC Test Implementation:-

This is under the BMAC_testing folder. There is a Kiel Project that can be executed to run a simple program using B-MAC.
This setup consists of 2 nodes running on a particular channel as specified in the variable MY_CHANNEL. This runs the B-MAC task on top of the Nano-RK RTOS to facilitate communication between the 2 nodes.
The aim was to run tests on this simple setup and see packet loss occurring to make a quatitative comparison with that of our RT_LINK implementation.

2)RT_LINK Implementaion:-

This is under the senseX folder. There are 3 Keil projects one for the coordinator(MRK_coordinator) and 2 for the followers/Mobile nodes(MRK_MOBILE,MRK_MOBILE2).TO set it up run the keil projects idependently and burn to 3 different mbeds.The RT-Link implementation runs on top of the Nano-RK RTOS.The behavior is the same as that of the B-MAC impementation but with a few modifications.In the coordinator main.cc file the rtl_init() function is set as 'coordinator'.
While in the mobile nodes they are set as 'mobile'.

  Furthermore the channel setup is as follows:-
  
    *Coordinator:
                 MY_TX_SLOT  22        //Transmission slot (for synchronization)
                 MY_RX_SLOT1 9         //Reception slot for mobile 1
                 MY_RX_SLOT2 17        //Reception slot for mobile 2
                 
    *Mobile 1:
                MY_TX_SLOT  9         //Transmission slot to Coordinator
                MY_RX_SLOT1  22       //Reception slot from Coordinator (for synchronization)
                MY_RX_SLOT2  17       //Reception Slot for mobile 2
                
    *Mobile 2:
                MY_TX_SLOT  17        //Transmission slot to Coordinator
                MY_RX_SLOT1  22       //Reception slot from Coordinator (for synchronization)
                
This current setup can be modified as per the requirements of the individual for further testing. Furthermore the setup is such that the mobile node need not be in the vicinity of the coordinator but can sync with another mobile node. On the whole a chain is setup which on the whole will be in sync with a coordinator.

