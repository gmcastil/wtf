RM = /usr/bin/rm
MKDIR = /usr/bin/mkdir
AR = /usr/bin/ar
CC = /usr/bin/gcc
CFLAGS = -Wall -pedantic -Wextra -ansi -O0 -g

INCDIR = include
LIBDIR = lib

clean:
	@$(RM) -vf squares.o libsquares.a libsquares.so
	@$(RM) -vf static_demo shared_demo
	@$(RM) -rvf $(LIBDIR)

static:
	@$(MKDIR) -pv $(LIBDIR)
	@$(RM) -vf static_demo
	@$(RM) -vf $(LIBDIR)/libsquares.a
	$(CC) $(CFLAGS) -c squares.c -o squares.o
	$(AR) -cvq $(LIBDIR)/libsquares.a squares.o
	@$(RM) -fv squares.o
	$(CC) $(CFLAGS) -I$(INCDIR) -o static_demo main.c -lsquares -L$(LIBDIR)

shared:
	@$(MKDIR) -pv $(LIBDIR)
	@$(RM) -vf shared_demo
	@$(RM) -vf $(LIBDIR)/libsquares.so
	$(CC) $(CFLAGS) -fPIC -c squares.c -o squares.o
	$(CC) $(CFLAGS) -shared squares.o -o $(LIBDIR)/libsquares.so
	@$(RM) -vf squares.o
	$(CC) $(CFLAGS) -I$(INCDIR) -L$(LIBDIR) -o shared_demo main.c -lsquares

all: static shared

