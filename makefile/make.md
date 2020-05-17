执行make时先找makefile，后找Makefile。
语法格式：
target:files
[tab] cmd

# Demo：

## 原始版本：

```makefile
main.out:main.o tool1.o tool2.o
	gcc main.o tool1.o tool2.o -o main.out
main.o:main.c
	gcc main.c -c -o main.o
tool1.o:tool1.c
	gcc tool1.c -c -o tool1.o
tool2.o:tool2.c
	gcc tool2.c -c -o tool2.o
PHONY:clean
clean:
	rm -rf *.o *.out
```



## 加入变量：

```makefile
OBJS = main.o tool1.o tool2.o
CC = gcc
CFLAGS += -c  -Wall -g
main.out:$(OBJS)
	$(CC) $(OBJS) -o main.out
main.o:main.c
	$(CC) main.c  $(CFLAGS) -o main.o
tool1.o:tool1.c
	$(CC) tool1.c $(CFLAGS) -o tool1.o
tool2.o:tool2.c
	$(CC) tool2.c $(CFLAGS) -o tool2.o
clean:
	$(RM) *.o *.out -r
```



## 自动变量：

```makefile
OBJS = main.o tool1.o tool2.o
CC = gcc
CFLAGS += -c  -Wall -g

main.out:$(OBJS)
	$(CC) $(OBJS) -o $@
main.o:main.c
	$(CC) $^  $(CFLAGS) -o $@
tool1.o:tool1.c
	$(CC) $^  $(CFLAGS) -o $@
tool2.o:tool2.c
	$(CC) $^  $(CFLAGS) -o $@
clean:
	$(RM) *.o *.out -r
```

$@ ：表示目标文件
$^ ：表示所有的依赖文件



## 隐含规则：

```makefile
OBJS = main.o tool1.o tool2.o
CC = gcc
CFLAGS += -c  -Wall -g

main.out:$(OBJS)
	$(CC) $(OBJS) -o $@
%.o:%.c
	$(CC) $^  $(CFLAGS) -o $@
clean:
	$(RM) *.o *.out -r
```



生成若干个可执行文件

如果你的Makefile需要一口气生成若干个可执行文件，但你只 想简单地敲一个make完事，并且，所有的目标文件都写在一个Makefile中，那么你可以使用“伪目标”这个 特性：

```makefile
all : prog1 prog2 prog3
.PHONY : all

prog1 : prog1.o utils.o
    cc -o prog1 prog1.o utils.o

prog2 : prog2.o
    cc -o prog2 prog2.o

prog3 : prog3.o sort.o utils.o
    cc -o prog3 prog3.o sort.o utils.o
```



# Refrence：

1. [linux make makefile 内置变量 默认变量](https://blog.csdn.net/whatday/article/details/104079644)
3. [makefile $@, $^, $<, $?](https://www.cnblogs.com/gamesun/p/3323155.html)
4. [跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/index.html)
4. [Makefile中.PHONY的作用](https://www.cnblogs.com/idorax/p/9306528.html)
5. [Makefile 中:= ?= += =的区别](https://www.cnblogs.com/zgq0/p/8716150.html)
6. [Makefile的静态模式%.o : %.c](https://blog.csdn.net/u012351051/article/details/88600562)

