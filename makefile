main.exe: main.o fichiers.o fir.o iir.o integration.o mesure.o lecture.o affichage.o 
	gcc main.o fichiers.o fir.o iir.o integration.o mesure.o lecture.o affichage.o -lm -o main.exe

main.o: main.c
	gcc -g -c main.c

fichiers.o: fichiers.c fichiers.h
	gcc -g -c fichiers.c

fir.o: fir.c fir.h
	gcc -g -c fir.c

iir.o: iir.c iir.h
	gcc -g -c iir.c

integration.o: integration.c integration.h
	gcc -g -c integration.c

mesure.o: mesure.c mesure.h
	gcc -g -c mesure.c

lecture.o: lecture.c lecture.h
	gcc -g -c lecture.c

affichage.o: affichage.c affichage.h
	gcc -g -c affichage.c

clean:
	rm *.o *.exe
