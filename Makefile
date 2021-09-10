CROSS=m68k-atari-mint-

CC=$(CROSS)gcc
CFLAGS=-O2 -fomit-frame-pointer -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Werror
LDFLAGS = -s

all: hyp2gdos.ttp

OBJS = \
	main.o \
	hyp.o \
	lh5d.o \
	util.o \
	$(empty)

hyp2gdos.ttp: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lgem

$(OBJS): hyp.h hyp2gdos.h

clean::
	rm -f *.o hyp2gdos.ttp *.pdb
