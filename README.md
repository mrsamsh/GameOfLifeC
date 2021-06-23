# GameOfLifeC

This is a Game of Life Simulation written in C using SDL2 library.

It requires SDL2 to be preinstalled. Currently the makefile works on Mac and
Linux.

You can play with screensize and cell side in config file. Just copy config.txt.sample to config.txt and edit the latter. Display is the number of the screen you want to use if you have multiple monitors. If you want HiDPI to be used, set it to 1, or 0 to disable it. Note that this should be set according to the display you use.

Just cd to project folder and type:
```bash
$ make run
```
