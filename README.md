# Thread Scheduling Simulation

Programming exercise in the course of the lecture _Basic Principles: Operating Systems and System Software_ (IN0009) at the Technical University of Munich.\

Syntax: `threadsched -n <N Threads> -t <time step> -q <time quantum> -a <algorithm>`  

\<N Threads>:    defines how many threads will be run in the simulation\
\<time step>:    defines ...\
\<time quantum>: defines the time steps in which a clock is counted up (starting with 0).\
\<algorithm>:    defines the scheduling algorithm used to handle the threads

**Round Robin (RR)**\
This algorithm ignores the priorities and executes the threads available at any given time one by one, each for the corresponding time quantum. When a new thread is started, it is added to the end of the current list
of calculation-ready threads.

**Priority Round Robin (PRR)**\
This algorithm is designed to take priorities into account and strictly execute the higher priority threads before the lower priority threads. Round robin scheduling is used within a priority.

**Shortest Remaining Time Next (SRTN)**
This algorithm prioritizes the threads according to their remaining runtime, with the one with the shortest remaining runtime receiving the CPU first. The system works preemptively: if a new thread is added that has a shorter remaining runtime, the thread currently running is interrupted and the new one is executed; that is, if the remaining runtime is the same, the active thread continues running.


## Example outputs

```
$ ./threadsched −n 3 −t 10 −q 50 −a RR
1 100 200
1 200 200
1 300 200
  
  Time | 1 2 3 4 5 6 7 8 9 10
−−−−−−−+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
000000 |
000010 |
000020 |
000030 |
000040 |
000050 |
000060 |
000070 |
000080 |
000090 |
000100 | 1
000110 | 1
000120 | 1
000130 | 1
000140 | 1
000150 | 1
000160 | 1
000170 | 1
000180 | 1
000190 | 1
000200 |   2
000210 |   2
000220 |   2
000230 |   2
000240 |   2
000250 | 1
000260 | 1
000270 | 1
000280 | 1
000290 | 1
000300 |   2
000310 |   2
000320 |   2
000330 |   2
000340 |   2
000350 |     3
000360 |     3
000370 |     3
000380 |     3
000390 |     3
000400 | 1
000410 | 1
000420 | 1
000430 | 1
000440 | 1
000450 |   2
000460 |   2
000470 |   2
000480 |   2
000490 |   2
000500 |     3
000510 |     3
000520 |     3
000530 |     3
000540 |     3
000550 |   2
000560 |   2
000570 |   2
000580 |   2
000590 |   2
000600 |     3
000610 |     3
000620 |     3
000630 |     3
000640 |     3
000650 |     3
000660 |     3
000670 |     3
000680 |     3
000690 |     3
```

```
$ ./threadsched −n 4 −t 10 −q 50 −a PRR
1 50 200
1 100 200
2 150 200
1 200 200

  Time | 1 2 3 4 5 6 7 8 9 10
−−−−−−−+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
000000 |
000010 |
000020 |
000030 |
000040 |
000050 | 1
000060 | 1
000070 | 1
000080 | 1
000090 | 1
000100 |   2
000110 |   2
000120 |   2
000130 |   2
000140 |   2
000150 | 1
000160 | 1
000170 | 1
000180 | 1
000190 | 1
000200 |   2
000210 |   2
000220 |   2
000230 |   2
000240 |   2
000250 |       4
000260 |       4
000270 |       4
000280 |       4
000290 |       4
000300 | 1
000310 | 1
000320 | 1
000330 | 1
000340 | 1
000350 |   2
000360 |   2
000370 |   2
000380 |   2
000390 |   2
000400 |       4
000410 |       4
000420 |       4
000430 |       4
000440 |       4
000450 | 1
000460 | 1
000470 | 1
000480 | 1
000490 | 1
000500 |   2
000510 |   2
000520 |   2
000530 |   2
000540 |   2
000550 |       4
000560 |       4
000570 |       4
000580 |       4
000590 |       4
000600 |       4
000610 |       4
000620 |       4
000630 |       4
000640 |       4
000650 |     3
000660 |     3
000670 |     3
000680 |     3
000690 |     3
000700 |     3
000710 |     3
000720 |     3
000730 |     3
000740 |     3
000750 |     3
000760 |     3
000770 |     3
000780 |     3
000790 |     3
000800 |     3
000810 |     3
000820 |     3
000830 |     3
000840 |     3
```

```
$ ./threadsched −n 3 −t 10 −q 50 −a SRTN
1 100 300
1 150 200
1 200 100

  Time | 1 2 3 4 5 6 7 8 9 10
−−−−−−−+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
000000 |
000010 |
000020 |
000030 |
000040 |
000050 |
000060 |
000070 |
000080 |
000090 |
000100 | 1
000110 | 1
000120 | 1
000130 | 1
000140 | 1
000150 |   2
000160 |   2
000170 |   2
000180 |   2
000190 |   2
000200 |     3
000210 |     3
000220 |     3
000230 |     3
000240 |     3
000250 |     3
000260 |     3
000270 |     3
000280 |     3
000290 |     3
000300 |   2
000310 |   2
000320 |   2
000330 |   2
000340 |   2
000350 |   2
000360 |   2
000370 |   2
000380 |   2
000390 |   2
000400 |   2
000410 |   2
000420 |   2
000430 |   2
000440 |   2
000450 | 1
000460 | 1
000470 | 1
000480 | 1
000490 | 1
000500 | 1
000510 | 1
000520 | 1
000530 | 1
000540 | 1
000550 | 1
000560 | 1
000570 | 1
000580 | 1
000590 | 1
000600 | 1
000610 | 1 
000620 | 1
000630 | 1
000640 | 1
000650 | 1
000660 | 1
000670 | 1
000680 | 1
000690 | 1
```
