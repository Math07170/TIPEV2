<<<<<<< HEAD
tipe:  grille.o fileprio.o TIPE.o affichage.o affichePPM.o carte.o genetique.o
		gcc TIPE.o fileprio.o grille.o affichage.o affichePPM.o carte.o genetique.o -o tipe -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
tipe-all:
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
		gcc -c affichePPM.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
		gcc -c genetique.c -lncurses -lmath -g -Wall -Wextra -fsanitize=undefined -lm
		gcc -c TIPE.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe -lncurses -lm
TIPE.o: TIPE.c affichage.h affichePPM.h grille.h fileprio.h carte.h genetique.h
		gcc -c TIPE.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
=======
tipe:  grille.o fileprio.o TIPE.o affichage.o carte.o genetique.o
		gcc TIPE.o fileprio.o grille.o affichage.o carte.o genetique.o -o tipe -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
tipe-all:
		gcc -c fileprio.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
		gcc -c grille.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
		gcc -c affichage.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
		gcc -c TIPE.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
		gcc TIPE.o fileprio.o grille.o affichage.o -o tipe -lncurses
TIPE.o: TIPE.c affichage.h grille.h fileprio.h carte.h genetique.h
		gcc -c TIPE.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
>>>>>>> parent of 28d5fe1 (commit)
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
affichage.o: affichage.c affichage.h grille.h fileprio.h
<<<<<<< HEAD
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
affichePPM.o: affichePPM.c affichePPM.h grille.h fileprio.h
		gcc -c affichePPM.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
=======
		gcc -c affichage.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
>>>>>>> parent of 28d5fe1 (commit)
fileprio.o: fileprio.c fileprio.h
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
carte.o: carte.c carte.h grille.h
<<<<<<< HEAD
		gcc -c carte.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
genetique.o: genetique.c grille.h carte.h
		gcc -c genetique.c -lncurses -lmath -g -Wall -Wextra -fsanitize=undefined -lm
=======
		gcc -c carte.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
genetique.o: genetique.c grille.h affichage.h carte.h
		gcc -c genetique.c -lncurses -lm -g -Wall -Wextra -fsanitize=undefined
>>>>>>> parent of 28d5fe1 (commit)
clean:
	rm -f *.o
	rm -f tipe
