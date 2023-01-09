tipe:  grille.o fileprio.o TIPE.o affichage.o
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe -lncurses
TIPE.o: TIPE.c grille.h affichage.c fileprio.h
		gcc -c TIPE.c -lncurses
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c -lncurses
fileprio.o: fileprio.c fileprio.h
			gcc -c fileprio.c -lncurses
clean:
	rm -f *.o
	rm -f tipe
