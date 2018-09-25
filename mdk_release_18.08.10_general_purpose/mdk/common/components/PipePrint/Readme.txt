Readme:

Example of full debug script located in scripts folder in the component.

Please create a custom debug script in your application with
the following content before the run command:

uart off
pipe LOS read @mvConsoleTxQueue
pipe LOS >>
pipe LRT read @lrt_mvConsoleTxQueue
pipe LRT >>
