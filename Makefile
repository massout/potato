CFILES = $(wildcard *.c lib/*.c)
OFILES = $(CFILES:.c=.o)
INCLUDE = $(include/*.h)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
CC = "aarch64-none-elf-gcc"
LD = "aarch64-none-elf-ld"
OBJCOPY = "aarch64-none-elf-objcopy"

all: clear kernel8.img

entry.o: boot/entry.S
	$(CC) $(GCCFLAGS) -c boot/entry.S -o entry.o 

%.o: %.c
	$(CC) $(GCCFLAGS) -c $< -o $@ -Iinclude

kernel8.img: entry.o $(OFILES)
	$(LD) -nostdlib entry.o $(OFILES) -T boot/link.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clear:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio
