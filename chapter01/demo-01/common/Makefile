CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

MYCFLAGS=
MYLDFLAGS=
MYLIBS=

AR= ar rcu
RANLIB= ranlib
RM= rm -rf

ERR_A= liberr.a
BASE_O= err.o


# Targets start here
a: $(ERR_A)


$(ERR_A): $(BASE_O)
	$(AR) $@ $(BASE_O)
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
	$(RM) $(ERR_A) $(BASE_O)

# Convenience targets
A= a
c89:
	$(MAKE) $(T) CC="gcc -std=c89"

.PHONY: a echo clean

err.o: err.c err.h
