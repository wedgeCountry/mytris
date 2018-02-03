mytris
------

A game console was designed to play tetris as a 4-player game on a small device.

#### Installation: ####
./amake

#### TODO: ####
- Hardware-specs
- bug-fix game_mechanics.c
- clean up menu.h
- implement highscore using EEPROM


#### Program Files: ####

Main.c

###### meta game e.g. for difficulty and speed settings
metagame.h

###### game menu navigation
menu.h

###### accessing the GLCD routines for KS0108 
icd.h

###### game mechanics: this is my TETRIS implementation
game_mechanics.h

