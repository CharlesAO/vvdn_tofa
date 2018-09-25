ShaveFifoComm

Supported Platform
===================
The component works with 2x5x chip.

Overview
==========
The component implements a master-slave communication on shaves using FIFOs. The scope of this is to move the scheduling of an algorithm from leon to a shave. This is useful mostly in cases where the processing must be line based and blocks leon too much. It is also useful in cases where the shave algorithm runs multiple stages which can't trivially be divided across shaves.

The user will choose a master shave and arbitrary number of slave shaves. The master will not know the number of slaves for increased configurability. All the slaves will wait for a task to execute.  The master will give tasks for the slaves. Once a new task appeared, one of the free slaves will process it and send a response to the master, that the task was executed successfully. The component supports multiple types of tasks, and the master shave can also act as a slave (can do some job until the slaves finish the tasks)

Implementation details
======================
The communication is implemented using shave Fifos. The benefits of using the fifos will be detailed later in this document. 
One task type will consist of the following:
   - a shave fifo to use for task transmission - used by master and slaves
   - a shave fifo to use for response reception - used by master and slaves
   - an init handler - used on the slaves
   - a run handler - this function will be executed on the slave if it receives a task
The master can create different types of tasks. Each slave can be configured individually with the list of tasks that it can execute (it can be a single type, or multiple types)

A task added by the master consists of a pointer to a structure. This structure is the parameter list of the task. This structure must be shared between the master and slave. The slave will use this strucutre in the task handler on the slave side. There will be no global data on the slaves, all the required information must be passed in the parameter structure. This is mandatory because the master will not know the shave which will execute the task. It can be any of the free slaves.
   

The component provides a Leon API to initialize the master and the slaves, to create and assign task types for the master, and to assign different types of tasks to the slaves
There is also a shave API which is used by the master to add tasks and wait for their completion. The slave functions in the component are private, the handling of task reception and response is handled inside the component. The user will just need to implement the handler of the task. 
The shave entry points of the master and slaves are also shared, the user must ensure that the shaves are started with these functions. 

Because the shave fifos support atomic reads, the slaves will never execute a task twice and there is no need for mutex protection. More than this, the shave can monitor a fifo with a pretty low power consumption (while most of its clock tree is turned off). This means that when the slaves have no task to process or the master is waiting for a response, the power consumption is minimized. Unfortunately, this mode is available only if the slave is executing only one task type. The reason for this is that the low power monitoring can monitor only one fifo. Multiplexing the fifos would be possible, however it creates problems when a slave is not able to execute a type of task it received. It needs to put it back to the fifo, but in this moment the order of the tasks will change. This is why we preferred to go with this limitation for now. If a user wants to run multiple task types with one slave, he can multiplex the tasks manually by adding a type field to the parameter structure. The users handler will then choose the actual function to be called. 

