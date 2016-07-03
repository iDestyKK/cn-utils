# Generic Mass Bin Makefile
#
# This directory contains C and C++ source files for mass compiling executables in "../bin"
# The purpose of these files is to have tools that I may use frequently...
# The twist though is that I programmed these myself. Hah. :P
#
# ~Clara

# Specify Parameters
CC = gcc
CFLAGS = --std=gnu89
EPATH = ./../bin

all: $(EPATH)/create $(EPATH)/hex $(EPATH)/lower $(EPATH)/upper $(EPATH)/cn_render $(EPATH)/fl $(EPATH)/randstr $(EPATH)/this

# Perform Awesomeness...
$(EPATH)/create:
	$(CC) $(CFLAGS) -o $(EPATH)/create create.c

$(EPATH)/hex:
	$(CC) $(CFLAGS) -o $(EPATH)/hex hex.c

$(EPATH)/lower:
	$(CC) $(CFLAGS) -o $(EPATH)/lower lower.c lib/handyc.c

$(EPATH)/upper:
	$(CC) $(CFLAGS) -o $(EPATH)/upper upper.c lib/handyc.c

$(EPATH)/cn_render:
	$(CC) $(CFLAGS) -o $(EPATH)/cn_render cn_render.c lib/cn_vec.c

$(EPATH)/fl:
	$(CC) $(CFLAGS) -o $(EPATH)/fl fl.c lib/cn_vec.c

$(EPATH)/randstr:
	$(CC) $(CFLAGS) -o $(EPATH)/randstr randstr.c

$(EPATH)/this:
	$(CC) $(CFLAGS) -o $(EPATH)/this this.c

#Because I am sick of typing this command over and over again...
clean:
	rm $(EPATH)/*
