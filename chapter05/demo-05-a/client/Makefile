CXX= g++ -std=c++11
CXXFLAGS= -g -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

STATIC_ROOT= ..
DYNAMIC_ROOT= /usr/local/thirdparty

MYCFLAGS= -I$(STATIC_ROOT)
MYLDFLAGS= -L$(STATIC_ROOT)/common -L$(STATIC_ROOT)/proto -L$(DYNAMIC_ROOT)/protobuf/lib
MYLIBS= -lerr -lcal -lprotobuf -lpthread

ECHO_CLIENT_T= echo_client
ECHO_CLIENT_O= echo_client.o

# Targets start here
t: $(ECHO_CLIENT_T)

$(ECHO_CLIENT_T): $(ECHO_CLIENT_O)
	$(CXX) -o $@ $(LDFLAGS) $(ECHO_CLIENT_O) $(LIBS)

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
