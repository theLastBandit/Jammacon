# Gerbers & Firmware

|Board|Gerber|Firmware|
|---|---|---|
|JammaCon|JammaCon.zip| |
|SNES & NES|JC-SNES_th.zip| |
|SNES & NES (SMD Version)|JC-SNES.zip| |
|Saturn|JC-Saturn_th.zip| |
|Saturn (SMD Version)|JC-Saturn.zip| |
|Megadrive|JC-Megadrive.zip| |
|Megadrive 6|JC-Megadrive6.zip|megadrive.jed|
|PC-Engine|JC-PCEngine_th.zip| |
|PC-Engine (SMD Version)|JC-PCEngine.zip| |
|Playstation|JC-Playstation.zip|playstation.hex|
|Gamecube & N64|JC-GCN64.zip|gcn64.hex|
|XBOX|JC-XBOX.zip|xbox.hex|
|Playstation 3|JC-PS3.zip|ps3.hex|
|CPS Kick adapter|JC-CPS.zip| |

# Programming

## ATMEGA328 & ATMEGA32U4 
These can be programmed using **avrdude** and a cheap USBAsp compatible programmer. 

The XBOX & PS3 adapters have headers to enable in-circuit programming of the ATMega32U4's - the fuse settings are the defaults.

The ATMega328's are programmed out of circuit and require the fuses to be set as LOW=0xFF, HIGH=0xD9

## XC9536XL

I use Xilinx Impact along with a JTAG programmer to progam these out of circuit. Cheaper alternatives such as a Raspberry PI + OpenOCD should also work, although I've never attempted this.  
