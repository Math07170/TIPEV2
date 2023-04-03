tipe:  grille.o fileprio.o TIPE.o affichage.o carte.o
		gcc TIPE.o fileprio.o grille.o affichage.o carte.o -o tipe -lncurses -g -Wall -Wextra -fsanitize=undefined
tipe-all:
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined
		gcc -c TIPE.c -lncurses -g -Wall -Wextra -fsanitize=undefined
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe -lncurses
TIPE.o: TIPE.c affichage.h grille.h fileprio.h
		gcc -c TIPE.c -lncurses -g -Wall -Wextra -fsanitize=undefined
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined
fileprio.o: fileprio.c fileprio.h
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined
carte.o: carte.c carte.h grille.h
		gcc -c carte.c -lncurses -g -Wall -Wextra -fsanitize=undefined
clean:
	rm -f *.o
	rm -f tipe
