
kernel := build/kernel.bin
iso := build/hello.iso

linker_script := linker.ld
grub_cfg := boot/grub.cfg
assembly_source_files := $(wildcard *.asm)
assembly_object_files := $(patsubst %.asm, build/%.o, $(assembly_source_files))
c_source_files := $(wildcard *.c)
c_object_files := $(patsubst %.c, build/%.o, $(c_source_files))

CFLAGS = -fno-pic -static -fno-builtin -fno-strict-aliasing -fno-stack-protector -O1 -Wall -MD -ggdb -m32 -fno-omit-frame-pointer -Werror 

target ?= hello

# If the makefile can't find QEMU, specify its path here
# QEMU = qemu-system-i386

# Try to infer the correct QEMU
ifndef QEMU
QEMU = $(shell if which qemu > /dev/null; \
	then echo qemu; exit; \
	elif which qemu-system-i386 > /dev/null; \
	then echo qemu-system-i386; exit; \
	elif which qemu-system-x86_64 > /dev/null; \
	then echo qemu-system-x86_64; exit; \
	else \
	qemu=/Applications/Q.app/Contents/MacOS/i386-softmmu.app/Contents/MacOS/i386-softmmu; \
	if test -x $$qemu; then echo $$qemu; exit; fi; fi; \
	echo "***" 1>&2; \
	echo "*** Error: Couldn't find a working QEMU executable." 1>&2; \
	echo "*** Is the directory containing the qemu binary in your PATH" 1>&2; \
	echo "*** or have you tried setting the QEMU variable in Makefile?" 1>&2; \
	echo "***" 1>&2; exit 1)
endif



# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

.PHONY: all clean run iso kernel doc disk

all: $(kernel)

.gdbinit: .gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

clean:
	rm -r build

qemu: $(iso)
	qemu-system-i386 -cdrom $(iso) -curses -vga std -serial file:serial.log

qemu-nox: $(iso)
	qemu-system-x86_64 -m 128 -cdrom $(iso) -vga std -no-reboot -nographic 

qemu-gdb: $(iso) .gdbinit 
	qemu-system-x86_64 -S $(QEMUGDB) -m 128 -cdrom $(iso) -curses -vga std -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset 

.PHONY: qemu-gdb-nox
qemu-gdb-nox: $(iso) .gdbinit
	qemu-system-x86_64 -S $(QEMUGDB) -m 128 -cdrom $(iso) -vga std -serial file:serial.log -no-reboot -no-shutdown -d int,cpu_reset -nographic

iso: $(iso)
	@echo "Done"

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/kernel.bin
	cp $(grub_cfg) build/isofiles/boot/grub
	grub2-mkrescue -d lib/grub/i386-pc -o $(iso) build/isofiles #2> /dev/null
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

