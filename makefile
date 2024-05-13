all: main.o movies.o menu.o searchFile.o
	gcc -g --std=gnu99 main.o movies.o menu.o searchFile.o -o movies_by_year

main.o: main.c
	gcc -g --std=gnu99 -c -Wall main.c
	
movies.o: movies.c
	gcc -g --std=gnu99 -c -Wall movies.c

menu.o: menu.c
	gcc -g --std=gnu99 -c -Wall menu.c

searchFile.o: searchFile.c
	gcc -g --std=gnu99 -c -Wall searchFile.c

clean:
	rm -f *.o movies_by_year