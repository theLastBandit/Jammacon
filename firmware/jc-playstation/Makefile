CC=avr-gcc
AS=$(CC)
LD=$(CC)

CPU=atmega328
CLOCK=16
AVRDUDE=avrdude -C"/etc/avrdude.conf" -F -p $(CPU) -P usb -c usbasp
CFLAGS=-Wall -mmcu=$(CPU) -Os -DF_CPU=$(CLOCK)000000L
LDFLAGS=-mmcu=$(CPU) -Wl,-Map=ps2-map.map

OBJS=controller.o main.o
PROG=ps2-$(CLOCK)Mhz

# RSTDISBL  WDTON  SPIEN  CKOPT  EESAVE  BOOTSZ1  BOOTSZ0  BOOTRST
#    1        1      0      1      1        0        0        1
HFUSE=0xD9

# BODLEVEL  BODEN  SUT1  SUT0  CKSEL3  CKSEL2  CKSEL1  CKSEL0
#    1        1      1    0      0       1       0       0
#

LFUSE=0xFF

all: $(PROG).hex

clean:
	rm -f $(PROG).elf $(PROG).hex ps2-map.map $(OBJS)

$(PROG).elf: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(PROG).elf

$(PROG).hex: $(PROG).elf
	avr-objcopy -j .data -j .text -O ihex $(PROG).elf $(PROG).hex
	avr-size $(PROG).elf

flash: $(PROG).hex
	$(AVRDUDE) -Uflash:w:$< 

fuse:
	$(AVRDUDE) -e -Uhfuse:w:$(HFUSE):m -Ulfuse:w:$(LFUSE):m 

erase:
	$(AVRDUDE) -B 10.0 -e

reset:
	$(AVRDUDE) -B 10.0

%.o: %.S
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
