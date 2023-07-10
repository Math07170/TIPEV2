tipe:  grille.o fileprio.o TIPE.o affichage.o affichePPM.o carte.o genetique.o
		gcc TIPE.o fileprio.o grille.o affichage.o affichePPM.o carte.o genetique.o -o tipe -O2 -lncurses -lpthread -g -Wall -Wextra -fsanitize=undefined -lm -march=native
TIPE.o: TIPE.c affichage.h affichePPM.h grille.h fileprio.h carte.h genetique.h
		gcc -c TIPE.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm
grille.o: grille.c grille.h
		gcc -c grille.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -lpthread -march=native
affichage.o: affichage.c affichage.h grille.h fileprio.h
		gcc -c affichage.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -march=native
affichePPM.o: affichePPM.c affichePPM.h grille.h fileprio.h
		gcc -c affichePPM.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -march=native
fileprio.o: fileprio.c fileprio.h
		gcc -c fileprio.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -march=native
carte.o: carte.c carte.h grille.h affichePPM.h
		gcc -c carte.c -lncurses -g -Wall -Wextra -fsanitize=undefined -lm -march=native
genetique.o: genetique.c grille.h carte.h
		gcc -c genetique.c -lncurses -lmath -g -Wall -Wextra -fsanitize=undefined -lm -lpthread -march=native
clean:
	rm -f *.o
	rm -f tipe
