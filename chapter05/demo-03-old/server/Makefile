CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

STATIC_ROOT= ..
DYNAMIC_ROOT= /usr/local/thirdparty/zlog-1.2

MYCFLAGS= -I$(STATIC_ROOT) -I$(DYNAMIC_ROOT)/include
MYLDFLAGS= -L$(STATIC_ROOT)/common -L$(DYNAMIC_ROOT)/lib -Wl,-rpath=$(DYNAMIC_ROOT)/lib
MYLIBS= -lerr -llog -lzlog -lpthread

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
