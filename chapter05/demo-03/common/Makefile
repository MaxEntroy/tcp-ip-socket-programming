CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

DYNAMIC_ROOT= /usr/local/thirdparty/zlog-1.2

MYCFLAGS= -I$(DYNAMIC_ROOT)/include
MYLDFLAGS=
MYLIBS=

AR= ar rcu
RANLIB= ranlib
RM= rm -rf

ERR_A= liberr.a
ERR_O= err.o

LOG_A= liblog.a
LOG_O= log.o

ALL_A= $(ERR_A) $(LOG_A)

# Targets start here
all: $(ALL_A)

$(ERR_A): $(ERR_O)
	$(AR) $@ $(ERR_O)
	$(RANLIB) $@

$(LOG_A): $(LOG_O)
	$(AR) $@ $(LOG_O)
	$(RANLIB) $@

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(ERR_A) $(ERR_O) $(LOG_A) $(LOG_O)

# Convenience targets
c89:
	$(MAKE) $(ALL_A) CC="gcc -std=c89"

.PHONY: a echo clean

err.o: err.c err.h
log.o: log.c log.h
