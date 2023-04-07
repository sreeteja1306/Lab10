CC = gcc
CFLAGS = -Wall -g
OBJS = Lab10.o
EXECS = Lab10
TARGETFOLDER = sample.txt

build : $(EXECS)
	./$^ $(TARGETFOLDER)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(EXECS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm $^
