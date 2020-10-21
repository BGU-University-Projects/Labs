#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: smile

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
smile: out.o 
	gcc -m32 -g -Wall -o smile out.o 

# Depends on the source and header files
out.o: task1.c
	gcc -g -Wall -m32 -ansi -c -o out.o task1.c	

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o smile
