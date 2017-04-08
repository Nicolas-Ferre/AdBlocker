CFLAGS = -Wall
EXEC_NAME = MyAdBlocker
LIBS = 

C_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c,obj/%.o,$(C_FILES))

$(EXEC_NAME) : prog

prog : $(OBJ_FILES)
	@gcc $(CFLAGS) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)
	@echo "Done."

obj/%.o : src/%.c
	@mkdir -p obj
	@gcc -o $@ -c $<
	@echo $<

clean :
	@rm -rf obj/*.o
	@rm -f $(EXEC_NAME)
	@echo "Done."