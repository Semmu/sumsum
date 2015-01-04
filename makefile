CC=gcc
FLAGS=`sdl-config --cflags` -Wall -Og
LIBS=`sdl-config --libs` -lSDL_image -lSDL_ttf -lm

all: ./main.x
	@echo -n " - running"
	@./main.x
	@echo " [OK]"

./main.x: ./build/globals.o ./build/game_methods.o ./build/state_main_menu.o ./build/state_playing.o ./build/state_victory.o ./build/state_defeat.o ./build/state_view_highscores.o ./build/main.o
	@echo -n " - linking"
	@$(CC) ./build/*.o $(LIBS) -o ./main.x
	@echo " [OK]"

./build/state_main_menu.o: state_main_menu.c state_main_menu.h
	@echo -n " - compiling state_main_menu.c"
	@$(CC) -c ./state_main_menu.c $(FLAGS) -o ./build/state_main_menu.o
	@echo " [OK]"

./build/state_playing.o: state_playing.c state_playing.h
	@echo -n " - compiling state_playing.c"
	@$(CC) -c ./state_playing.c $(FLAGS) -o ./build/state_playing.o
	@echo " [OK]"

./build/state_victory.o: state_victory.c state_victory.h
	@echo -n " - compiling state_victory.c"
	@$(CC) -c ./state_victory.c $(FLAGS) -o ./build/state_victory.o
	@echo " [OK]"

./build/state_defeat.o: state_defeat.c state_defeat.h
	@echo -n " - compiling state_defeat.c"
	@$(CC) -c ./state_defeat.c $(FLAGS) -o ./build/state_defeat.o
	@echo " [OK]"

./build/state_view_highscores.o: state_view_highscores.c state_view_highscores.h
	@echo -n " - compiling state_view_highscores.c"
	@$(CC) -c ./state_view_highscores.c $(FLAGS) -o ./build/state_view_highscores.o
	@echo " [OK]"

./build/main.o: main.c
	@echo -n " - compiling main.c"
	@$(CC) -c ./main.c $(FLAGS) -o ./build/main.o
	@echo " [OK]"

./build/globals.o: globals.c globals.h
	@echo -n " - compiling globals.c"
	@$(CC) -c ./globals.c $(FLAGS) -o ./build/globals.o
	@echo " [OK]"

./build/game_methods.o: game_methods.c game_methods.h
	@echo -n " - compiling game_methods.c"
	@$(CC) -c ./game_methods.c $(FLAGS) -o ./build/game_methods.o
	@echo " [OK]"

clean:
	@echo -n " - cleaning up"
	@rm -f ./build/*
	@rm -f ./main.x
	@echo " [OK]"

re: clean all