
# Internet Of Things TOSSIM simulation  

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/5ade59ba8f024a19b5fd1a605cd25421)](https://www.codacy.com/manual/ataeiamirhosein/Tossim?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ataeiamirhosein/Tossim&amp;utm_campaign=Badge_Grade)

`tossim` is a simulator for tinyos that has has only CLI and you can see a snapshot of it on the below picture.  

![tossim schem](https://github.com/ataeiamirhosein/Tossim/blob/master/tossim.png)  

# Report and Result:  

we have three important file type in this simulation project which described below:  

in `.h` file we confine the stracture of functions.

also we have two file with `.nc` that one of them with name of describe `appC` and `C` files.

in `~appC.nc` file we have configuration and implementation of things that is in project. (e.g. timer)
for `~C.nc` file we have all the program code here for used module interfaces and running events.

## Result  

we can see the result of tossim below in CLI  

![screenshot from result of tossim](https://github.com/ataeiamirhosein/Tossim/blob/master/tos.png)  

## TOSSIM simulator  

TOSSIM simulates entire TinyOS applications. It works by replacing components with simulation implementations. The level at which components are replaced is very flexible: for example, there is a simulation implementation of millisecond timers that replaces HilTimerMilliC, while there is also an implementation for atmega128 platforms that replaces the HPL components of the hardware clocks. The former is general and can be used for any platform, but lacks the fidelity of capturing an actual chip's behavior, as the latter does.  

## including files of this repo for running simulation

- sendAck.h
- sendAckAppC.nc
- sendAckC.nc
- topology.txt

Finally, we can use :~$ `make micaz sim` command to compile and build files for tossim simulation after that run the python simulation file

## micaz mote  

![micaz](https://github.com/ataeiamirhosein/Tossim/blob/master/micaz.png)  

#
powered by [iotconet](https://www.github.com/iotconet)  
<https://www.iotco.net/en>
