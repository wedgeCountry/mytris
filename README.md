mytris
------

This game console was designed to play tetris as a 4-player game on a small device.

### Installation: ###
./amake

### TODO: ###
- Hardware-specs
- bug-fix game_mechanics.c
- clean up menu.h
- implement highscore using EEPROM


### Files and dependencies: ###

Main.c
|definitions.h
|ctrl.h
|menu.h
|modules.h
|lcd.h

## meta game e.g. for difficulty and speed settings
metagame.h
|definitions.h
|ctrl.h
|interface.h
|game_mechanics.h

## game menu navigation
menu.h
|definitions.h
|ctrl.h
|game.h

## accessing the GLCD routines for KS0108 
lcd.h
|definitions.h
interface.h
|definitions.h
|lcd.h

## game mechanics: this is my TETRIS implementation
game_mechanics.h
|definitions.h
|interface.h

