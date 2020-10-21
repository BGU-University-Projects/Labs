all: exec

exec: LineParser.c task4.c
	gcc -g -m32 -c -o LineParser.o LineParser.c
	gcc -g -m32 -c -o task4.o task4.c
	gcc -g -m32  LineParser.o task4.o  -o task4
	rm LineParser.o task4.o

#exec: looper.c
#		gcc -g -m32 -c -o looper.o looper.c
#		gcc -g -m32 looper.o  -o shell
.PHONY: clean
clean:
	rm -rf ./*.o task4
	#rm -rf ./*.o looper
