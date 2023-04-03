tipe:  grille.o fileprio.o TIPE.o affichage.o carte.o quickimage.o
		gcc TIPE.o fileprio.o grille.o affichage.o carte.o quickimage.o -o tipe -lncurses
tipe-all:
		gcc -c fileprio.c -lncurses
		gcc -c grille.c -lncurses
		gcc -c affichage.c -lncurses
		gcc -c TIPE.c -lncurses
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe -lncurses
TIPE.o: TIPE.c affichage.h grille.h fileprio.h
		gcc -c TIPE.c -lncurses
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c -lncurses
fileprio.o: fileprio.c fileprio.h
		gcc -c fileprio.c -lncurses
carte.o: carte.c carte.h grille.h quickimage.h
		gcc -c carte.c -lncurses
quickimage.o : quickimage.c quickimage.h
		gcc -c quickimage.c -lncurses
clean:
	rm -f *.o
	rm -f tipe
