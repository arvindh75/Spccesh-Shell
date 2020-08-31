CFLAGS = -g -Wall

spccesh: main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o str_util.o
	gcc -o kishmish $(CFLAGS) main.o cd.o prompt.o echo.o exit.o ls.o pwd.o setname.o str_util.o

clean:
	rm -f *.o kishmish

# DONOT USE THESE RULES DIRECTLY
cd.o: cd.c cd.h
	gcc $(CFLAGS) -c cd.c

prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c

echo.o: echo.c echo.h
	gcc $(CFLAGS) -c echo.c

ls.o: ls.c ls.h
	gcc $(CFLAGS) -c ls.c

pwd.o: pwd.c pwd.h
	gcc $(CFLAGS) -c pwd.c

exit.o: exit.c exit.h
	gcc $(CFLAGS) -c exit.c

str_util.o: str_util.c str_util.h
	gcc $(CFLAGS) -c str_util.c

setname.o: setname.c setname.h
	gcc $(CFLAGS) -c setname.c

main.o: main.c 
	gcc $(CFLAGS) -c main.c