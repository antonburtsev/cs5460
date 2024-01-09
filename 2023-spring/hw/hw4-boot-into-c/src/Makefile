
kernel := build/kernel.bin
iso := build/hello.iso

linker_script := linker.ld
grub_cfg := boot/grub.cfg
assembly_source_files := $(wildcard *.asm)
assembly_object_files := $(patsubst %.asm, build/%.o, $(assembly_source_files))
c_source_files := $(wildcard *.c)
c_object_files := $(patsubst %.c, build/%.o, $(c_source_files))

CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -O1 -Wall -MD -ggdb -m32 -fno-omit-frame-pointer -Werror -nostdlib 

target ?= hello

.PHONY: all clean run iso kernel doc disk

all: $(kernel)

clean:
	rm -rf build

qemu: $(iso)
	qemu-system-x86_64 -cdrom $(iso) -curses -vga std -s -serial file:serial.log

qemu-nox: $(iso)
	qemu-system-x86_64 -m 128 -cdrom $(iso) -vga std -s -no-reboot -nographic 

qemu-gdb: $(iso)
	qemu-system-x86_64 -S -m 128 -cdrom $(iso) -curses -vga std -s -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset 

.PHONY: qemu-gdb-nox
qemu-gdb-nox: $(iso)
	qemu-system-x86_64 -S -m 128 -cdrom $(iso) -vga std -s -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset -nographic

iso: $(iso)
	@echo "Done"

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/kernel.bin
	cp $(grub_cfg) build/isofiles/boot/grub
	grub2-mkrescue -o $(iso) build/isofiles #2> /dev/null
	@rm -r build/isofiles

$(kernel): $(c_object_files) $(assembly_object_files) $(linker_script)
	ld -m elf_i386  -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files)

# compile C files
build/%.o: %.c
	@mkdir -p $(shell dirname $@)
	gcc $(CFLAGS) -c $< -o $@

# compile assembly files
build/%.o: %.asm
	@mkdir -p $(shell dirname $@)
	nasm -felf32 $< -o $@

