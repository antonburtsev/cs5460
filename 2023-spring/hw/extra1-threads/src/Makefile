CC = $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gas
LD = $(TOOLPREFIX)ld
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

CFLAGS := -Wall -g -D_GNU_SOURCE -pthread -lm -fno-pic -O2 -m32
#CFLAGS += -DVERBOSE_DEBUG -DVERBOSE_ASSERT
CFLAGS += -DNDEBUG

INCLUDES := -I. 

SRCS = \
	threads.c main.c\

OBJS = $(SRCS:.c=.o)

MAIN=threads


.PHONY: depend clean

all:    $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm -f *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^


