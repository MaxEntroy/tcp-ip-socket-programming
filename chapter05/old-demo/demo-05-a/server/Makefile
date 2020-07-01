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

ECHO_SERVER_T= echo_server
ECHO_SERVER_O= echo_server.o

# Targets start here
t: $(ECHO_SERVER_T)

$(ECHO_SERVER_T): $(ECHO_SERVER_O)
	$(CXX) -o $@ $(LDFLAGS) $(ECHO_SERVER_O) $(LIBS)

echo:
	@echo "CXX= $(CXX)"
	@echo "CXXFLAGS= $(CXXFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(ECHO_SERVER_T) $(ECHO_SERVER_O)

.PHONY: t echo clean

echo_server.o: echo_server.cc
