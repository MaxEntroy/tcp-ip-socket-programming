CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= ..

MYCFLAGS= -I$(ROOT)
MYLDFLAGS= -L$(ROOT)/common
MYLIBS= -lerr

ECHO_SERVER_T= echo_server
ECHO_SERVER_O= echo_server.o

# Targets start here
t: $(ECHO_SERVER_T)

$(ECHO_SERVER_T): $(ECHO_SERVER_O)
	$(CC) -o $@ $(LDFLAGS) $(ECHO_SERVER_O) $(LIBS)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(ECHO_SERVER_T) $(ECHO_SERVER_O)

# Convenience targets
T= t
c89:
	$(MAKE) $(T) CC="gcc -std=c89"

.PHONY: t echo clean

echo_server.o: echo_server.c
