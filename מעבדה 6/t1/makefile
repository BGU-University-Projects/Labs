all: exec

exec: LineParser.c task1.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task1.o task1.c
	gcc -g -m32  LineParser.o task1.o  -o task1
	rm LineParser.o task1.o

#exec: looper.c
#		gcc -g -m32 -c -o looper.o looper.c
#		gcc -g -m32 looper.o  -o task1
.PHONY: clean
clean:
	rm -rf ./*.o task1
	#rm -rf ./*.o looper
