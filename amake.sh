#!/bin/bash

## VORSICHT!! FUSES ANPASSEN

TARGET="Main"
SRC="$TARGET.c"
SRC_FOLDER="src"
MCU="atmega32"
DEVICE="m32"
CC="avr-gcc"
LFUSE="0xe1"
HFUSE="0xd9"


# clean
rm *.o

for cfile in $SRC_FOLDER/*; 
do
	name=${cfile%'.c'} # .c-endung entfernen
	endung=${cfile#"$name"} # echte endung herausfinden
	name=${name#"$SRC_FOLDER/"}  # prefix entfernen
	if [ "$endung" == ".c" ]
	then
		if [ "$name" != "$TARGET" ]
		then
			echo "compiling $name.o"
			# compile
			$CC $SRC_FOLDER/$name.c -c -o $name.o -Os -g -mmcu=$MCU
 			echo "--------------"
		fi	
	fi
done

echo "compiling $TARGET.o (target)"
$CC $SRC_FOLDER/$TARGET.c -c -o $TARGET.o -Os -g -mmcu=$MCU
echo "--------------"

echo "linking"
$CC *.o -o $TARGET.elf -mmcu=$MCU
echo "--------------"

echo "creating hex file"
avr-objcopy -j .text -j .data -O ihex $TARGET.elf $TARGET.hex
echo "--------------"

# eeprom file
#avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $TARGET.elf $TARGET-eeprom.hex

# listfile
#avr-objdump -h -S -j .text -j .data $TARGET.elf > $TARGET.lst

echo "flashing onto device"
#sudo avrdude -c avrispmkII -P usb -p $DEVICE -U flash:w:$TARGET.hex -U lfuse:w:$LFUSE:m -U hfuse:w:$HFUSE:m  
#sudo avrdude -c avrispmkII -P usb -p m8 -U eeprom:w:$TARGET.hex 
