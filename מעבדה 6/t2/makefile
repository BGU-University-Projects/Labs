all: exec

exec: LineParser.c task2.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task2.o task2.c
	gcc -g -m32  LineParser.o task2.o  -o task2
	rm LineParser.o task2.o

#exec: looper.c
#		gcc -g -m32 -c -o looper.o looper.c
#		gcc -g -m32 looper.o  -o shell
.PHONY: clean
clean:
	rm -rf ./*.o task2
	#rm -rf ./*.o looper
