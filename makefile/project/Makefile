all:clean main.out
	@echo "clean first then compile then link"

OBJS = main.o tool1.o tool2.o 
CC = gcc
CFLAGS += -c  -Wall -g

main.out:$(OBJS);@echo "link"
	$(CC) $^ -o $@
	
%.o:%.c;@echo "complie"
	$(CC) $^ $(CFLAGS) -o $@

PHONY:clean
clean:
	$(RM) *.o *.out -r
