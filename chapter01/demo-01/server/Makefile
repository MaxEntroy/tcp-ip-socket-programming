CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= ..

MYCFLAGS= -I$(ROOT)
MYLDFLAGS= -L$(ROOT)/common
MYLIBS= -lerr

MAIN_T= echo_server
MAIN_O= echo_server.o

# Targets start here
t: $(MAIN_T)

$(MAIN_T): $(MAIN_O)
	$(CC) -o $@ $(LDFLAGS) $(MAIN_O) $(LIBS)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(MAIN_T) $(MAIN_O)

# Convenience targets
T= t
c89:
	$(MAKE) $(T) CC="gcc -std=c89"

.PHONY: t echo clean

echo_server.o: echo_server.c
