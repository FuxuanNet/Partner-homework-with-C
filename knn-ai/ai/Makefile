SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

ALL: ai01.out

ai01.out: $(OBJ)
		gcc $< -o $@ -lm

$(OBJ): $(SRC)
		gcc -c $< -o $@

clean:
		rm -rf $(OBJ) ai01.out
