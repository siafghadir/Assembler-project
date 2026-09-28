# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = project_constants.h # Dependencies for everything
 EXE_DEPS = assembler.o  helper_functions.o table.o preproces.o first_exe.o second_exe.o encoding.o deal_with_data.o Errors.o deal_with_text.o lexer.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  assembler.c $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

preproces.o: preproces.c preproces.h $(GLOBAL_DEPS)
	$(CC) -c preproces.c $(CFLAGS) -o $@

first_exe.o: first_exe.c first_exe.h $(GLOBAL_DEPS)
	$(CC) -c first_exe.c $(CFLAGS) -o $@

second_exe.o: second_exe.c second_exe.h $(GLOBAL_DEPS)
	$(CC) -c second_exe.c $(CFLAGS) -o $@

encoding.o: encoding.c encoding.h $(GLOBAL_DEPS)
	$(CC) -c encoding.c $(CFLAGS) -o $@

deal_with_data.o: deal_with_data.c deal_with_data.h $(GLOBAL_DEPS)
	$(CC) -c deal_with_data.c $(CFLAGS) -o $@

table.o: table.c table.h $(GLOBAL_DEPS)
	$(CC) -c table.c $(CFLAGS) -o $@

helper_functions.o: helper_functions.c helper_functions.h $(GLOBAL_DEPS)
	$(CC) -c helper_functions.c $(CFLAGS) -o $@

Errors.o: Errors.c Errors.h $(GLOBAL_DEPS)
	$(CC) -c Errors.c $(CFLAGS) -o $@

deal_with_text.o: deal_with_text.c deal_with_text.h $(GLOBAL_DEPS)
	$(CC) -c deal_with_text.c $(CFLAGS) -o $@

lexer.o: lexer.c lexer.h $(GLOBAL_DEPS)
	$(CC) -c lexer.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
