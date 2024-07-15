ipli: main.o parser.o interpreter.o helper.o
	gcc -o ipli main.o parser.o interpreter.o helper.o

main.o: main.c
	gcc -c -o  main.o main.c

parser.o: parser.c
	gcc -c -o parser.o parser.c

interpreter.o: interpreter.c
	gcc -c -o interpreter.o interpreter.c

helper.o: helper.c
	gcc -c -o helper.o helper.c

run:
	./ipli programs_ipl/kalmpr.ipl

run1:
	./ipl programs_ipl/numbers.ipl

clean:
	rm -rf ipli main.o parser.o interpreter.o helper.o