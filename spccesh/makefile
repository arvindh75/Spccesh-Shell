spccesh: main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o exec_proc.o
	gcc -o spccesh -g -Wall main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o exec_proc.o

# DONOT USE THESE RULES DIRECTLY
clean:
	rm -f *.o spccesh

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

setname.o: setname.c setname.h
	gcc -g -Wall -c setname.c

exec_proc.o: exec_proc.c exec_proc.h
	gcc -g -Wall -c exec_proc.c

main.o: main.c 
	gcc -g -Wall -c main.c