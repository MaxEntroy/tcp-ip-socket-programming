# cpp setting
CXX= gcc -std=c++11
CXXFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

MYCFLAGS=
MYLDFLAGS=
MYLIBS=

AR= ar rcu
RANLIB= ranlib
RM= rm -f

CAL_A= libcal.a
CAL_O= cal_req.pb.o cal_res.pb.o

# proto setting
PROTO_ROOT= ./proto_file
CPP_ROOT= .

PROTOC= protoc
PROTOCFLAGS= -I$(PROTO_ROOT) --cpp_out=$(CPP_ROOT)

CAL_REQ= cal_req.proto
CAL_REQ_CPP= cal_req.pb.cc cal_req.pb.h

CAL_RES= cal_res.proto
CAL_RES_CPP= cal_res.pb.cc cal_res.pb.h

ALL_PROTO= req res

# common setting
RM= rm -f

# Targets start here
a: $(CAL_A)

$(CAL_A): $(CAL_O)
	$(AR) $@ $(CAL_O)
	$(RANLIB) $@

pb: $(ALL_PROTO)

req:
	$(PROTOC) $(PROTOCFLAGS) $(CAL_REQ)

res:
	$(PROTOC) $(PROTOCFLAGS) $(CAL_RES)


echo:
	@echo "CXX= $(CXX)"
	@echo "CXXFLAGS= $(CXXFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"
	@echo "PROTOC= $(PROTOC)"

clean:
	$(RM) $(CAL_A) $(CAL_O)

clean_pb_cpp:
	$(RM) $(CAL_REQ_CPP) $(CAL_RES_CPP)

.PHONY: a echo clean req res

cal_req.pb.o: $(CAL_REQ_CPP)
cal_res.pb.o: $(CAL_RES_CPP)
