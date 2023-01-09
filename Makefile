tipe:  grille.o fileprio.o TIPE.o affichage.o
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe
TIPE.o: TIPE.c grille.h affichage.c fileprio.h
		gcc -c TIPE.c
grille.o: grille.c grille.h
		gcc -c grille.c
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c
fileprio.o: fileprio.c fileprio.h
			gcc -c fileprio.c
clean:
	rm -f *.o
	rm -f tipe
