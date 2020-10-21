all: exec

exec: LineParser.c task3.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task3.o task3.c
	gcc -g -m32  LineParser.o task3.o  -o task3
	rm LineParser.o task3.o

#exec: looper.c
#		gcc -g -m32 -c -o looper.o looper.c
#		gcc -g -m32 looper.o  -o task3
.PHONY: clean
clean:
	rm -rf ./*.o task3
	#rm -rf ./*.o looper
