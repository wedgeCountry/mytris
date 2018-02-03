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
- metagame.h ###### meta game e.g. for difficulty and speed settings
- menu.h ###### game menu navigation
- icd.h ###### accessing the GLCD routines for KS0108 
- game_mechanics.h ###### game mechanics: this is my TETRIS implementation

