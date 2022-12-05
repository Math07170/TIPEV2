tipe:  grille.o fileprio.o TIPE.o
		gcc TIPE.o fileprio.o grille.o -o tipe 
tipe-graph: TIPE.o grille.o graphviz.o fileprio.o
		gcc fileprio.o grille.o TIPE.o -o tipe 
TIPE.o: TIPE.c grille.h graphviz.h
		gcc -c TIPE.c
grille.o: grille.c grille.h
		gcc -c grille.c
graphviz.o: graphviz.c graphviz.h grille.h
		gcc -c graphviz.c
fileprio.o: fileprio.c fileprio.h
			gcc -c fileprio.c
clean:
	rm -f *.o
	rm -f tipe
