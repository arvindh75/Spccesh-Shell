spccesh: main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o str_util.o
	gcc -o spccesh -g -Wall main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o str_util.o

# DONOT USE THESE RULES DIRECTLY
cd.o: cd.c cd.h
	gcc -g -Wall -c cd.c

prompt.o: prompt.c prompt.h
	gcc -g -Wall -c prompt.c

echo.o: echo.c echo.h
	gcc -g -Wall -c echo.c

ls.o: ls.c ls.h
	gcc -g -Wall -c ls.c

pwd.o: pwd.c pwd.h
	gcc -g -Wall -c pwd.c

exit.o: exit.c exit.h
	gcc -g -Wall -c exit.c

str_util.o: str_util.c str_util.h
	gcc -g -Wall -c str_util.c

setname.o: setname.c setname.h
	gcc -g -Wall -c setname.c

main.o: main.c 
	gcc -g -Wall -c main.c
