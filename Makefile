tipe:  grille.o fileprio.o TIPE.o affichage.o affichePPM.o carte.o genetique.o
		gcc TIPE.o fileprio.o grille.o affichage.o affichePPM.o carte.o genetique.o -o tipe -O2 -lncurses -lpthread -g -Wall -Wextra -fsanitize=undefined -lm
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
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -lpthread
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
affichePPM.o: affichePPM.c affichePPM.h grille.h fileprio.h
		gcc -c affichePPM.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
fileprio.o: fileprio.c fileprio.h
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
carte.o: carte.c carte.h grille.h
		gcc -c carte.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
genetique.o: genetique.c grille.h carte.h
		gcc -c genetique.c -lncurses -lmath -g -Wall -Wextra -fsanitize=undefined -lm -lpthread
clean:
	rm -f *.o
	rm -f tipe
