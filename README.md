# GameOfLifeC

This is a Game of Life Simulation written in C using SDL2 library.

It requires SDL2 to be preinstalled. Currently the makefile works on Mac and
Linux.

You can play with screensize and cell side in config file, it is arranged as:
```
width
height
cell_side
starting_cell_percentage(1-100)
frames_per_second
display_number (if you have multiple monitors attached)
```
You can copy config.txt.sample to config.txt and edit the latter.

Note that currently only high dpi monitors are supported.

Just cd to project folder and type:
```bash
$ make run
```
