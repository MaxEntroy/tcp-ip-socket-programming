CXX= g++ -std=c++11
CXXFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= ..

MYCFLAGS= -I$(ROOT)
MYLDFLAGS= -L$(ROOT)/common
MYLIBS= -lerr

ECHO_CLIENT_T= echo_client
ECHO_CLIENT_O= echo_client.o

# Targets start here
t: $(ECHO_CLIENT_T)

$(ECHO_CLIENT_T): $(ECHO_CLIENT_O)
	$(CC) -o $@ $(LDFLAGS) $(ECHO_CLIENT_O) $(LIBS)

echo:
	@echo "CXX= $(CC)"
	@echo "CXXFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(ECHO_CLIENT_T) $(ECHO_CLIENT_O)

.PHONY: t echo clean

echo_client.o: echo_client.cc
