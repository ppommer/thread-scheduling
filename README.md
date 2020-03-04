# Thread Scheduling

Syntax: `threadsched -n <N Threads> -t <time step> -q <time quantum> -a <algorithm>`  

The number of threads defines how many threads will be run in the simulation\
The time step defines ...\
The time quantum defines the time steps in which a clock is counted up (starting with 0).\


Example output for ...

`  Time | 1 2 3 4 5 6 7 8 9 10
−−−−−−−+−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−\
000000 |\
000100 | 1\
000200 | 1\ 
000300 | 1\ 
000400 |       4\
000500 |       4\
000600 |       4\`
