CROSS=m68k-atari-mint-

CC=$(CROSS)gcc
CFLAGS=-O2 -fomit-frame-pointer -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Werror
LDFLAGS = # -s

all: hyp2gdos.ttp

OBJS = \
	main.o \
	hyp.o \
	lh5d.o \
	file.o \
	font.o \
	util1.o \
	util2.o \
	util3.o \
	util4.o \
	$(empty)

hyp2gdos.ttp: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lgem

$(OBJS): hyp.h hyp2gdos.h

clean::
	rm -f *.o hyp2gdos.ttp *.pdb
