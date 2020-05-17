OBJS = main.o tool1.o tool2.o
CC = gcc
CFLAGS += -c  -Wall -g

main.out:$(OBJS)
	$(CC) $(OBJS) -o $@
%.o:%.c
	$(CC) $^  $(CFLAGS) -o $@

.PHONY:clean
clean:
	$(RM) *.o *.out -r
