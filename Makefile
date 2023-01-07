ARCH=i386
FORMAT=elf
NARCH=32

CC=$(ARCH)-$(FORMAT)-gcc
AS=nasm

SSRC = $(shell find src/ -name *.s)
SOUT = $(patsubst src/%.s, out/%.o, $(SSRC))

CSRC = $(shell find src/ -name *.c)
COUT = $(patsubst src/%.c, out/%.o, $(CSRC))

$(SOUT): out/%.o: src/%.s 
	@mkdir -p $(dir $@)
	$(AS) -f $(FORMAT)$(NARCH) $(patsubst out/%.o, src/%.s, $@) -o $@
$(COUT): out/%.o: src/%.c 
	@mkdir -p $(dir $@)
	$(CC) -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra -c $(patsubst out/%.o, src/%.c, $@) -o $@

build-sourimbase: $(SOUT) $(COUT)
	$(CC) -T linker.ld -o iso/boot/sourimbase.bin -ffreestanding -O2 -nostdlib -lgcc $(SOUT) $(COUT)
	grub-mkrescue iso -o sourim-base.iso