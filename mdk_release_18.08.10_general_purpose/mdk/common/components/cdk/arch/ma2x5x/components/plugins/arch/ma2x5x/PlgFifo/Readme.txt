PlgFifo

Supported Platform
===================
Myriad2 Ma2150 - This component works on Myriad2 silicon V2

Overview
==========
Serialize maximum 6 inputs source.
Assume X sources call Y consumer plugins. But that X consumer use same HW resources,
in consequence they are not allowed to run in parallel. The approach is to use a FIFO
list, and when the resources are free, call the trigger function of the FIFO plugin.
This function will call the associated consumer calbacks, if FIFO plugin has a frame
in the frame list.

Where X={1, …,6}, Y>0
