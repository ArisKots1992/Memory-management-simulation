# Memory management simulation

Memory management simulation in C++, Use different algorithms (Best-Fit, Worst-fit, Buddy) to place processes in memory and estimate their efficiency


####Best Fit: 
It has about the same mean and variance with the worst fit but exploits a little better the memory space.

####Worst Fit:
Not far from the Best Fit in essence is identical but choose the biggest and not the smallest gap for importing the VP.

####Buddy:
It has been clearly better dispersion because the gaps in memory puts spread so uniformly captured and stored in memory
(micro disadvantage is that) eg. for positioning 15 KB will commit 16.So a slight waste of memory.

---------------------------
###Implementation
In my implementation I have a big loop where each loop is 1 second. Now to cover the case that a VP did not fit in memory and has to enter the waiting qeueue
I didn't used shared memory for communication
manager -> generator because it is the responsibility of the generator
sending again vp_stop.Manager should itself
To correct the problem without the help of generator.
So in my implementation the manager receives either
fake or normally vp_stop.
fake it is called when a vp_stop comes for
a VP who are either still in the queue or not is left
in memory for as long needed.To implement this i have a
parallel queue_vp_stopper where they keep the difference vp start-vp stop
so I know how much time should wait in memory. Whenever
put / remove from the memory i renew the 2 queue in order to know
exactly when the manager to send the original vp_stop
to stay in memory just as much time should from
the given lifetime.

*Programmed in 2012*
