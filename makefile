#
#  2023 Robert Papagna
#
#  https://github.com/rpeepz/File2Image
#
#

BIN = File2Image
CC = clang -Iinc
CFLAG = -Wextra -Werror
LIB = -lavcodec -lavformat -lswscale -lpng
OBJ_PATH = obj
DEBUG = -Wall -g
BUILD = -std=c99 -pedantic -O2

OBJ = $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

SRC = main.c\
			file.c\
			ipow.c\
			create_png.c\
			read_png.c\
			run.c\
			header.c\
			complete.c

all: $(BIN)

re: fclean all

$(BIN):$(OBJ)
	$(CC) $(CFLAG) $(LIB) $(OBJ) -o $@ 

$(OBJ_PATH):
	mkdir -p $@

$(OBJ_PATH)/%.o: src/%.c inc/*.h | $(OBJ_PATH)
	$(CC) $(CFLAG) -c $< -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(BIN)
	rm -rf $(BIN).dSYM
	rm -f f2i*.png

debug: fclean
	$(CC) $(CFLAG) $(DEBUG) $(LIB) src/*.c -o $(BIN)

build: fclean
	$(CC) $(CFLAG) $(BUILD) $(LIB) src/*.c -o $(BIN)