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

MAIN_A= liberr.a
BASE_O= err.o


# Targets start here
a: $(MAIN_A)


$(MAIN_A): $(BASE_O)
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
	$(RM) $(MAIN_A) $(BASE_O)

# Convenience targets
T= t
c89:
	$(MAKE) $(T) CC="gcc -std=c89"

.PHONY: a echo clean clean_o

err.o: err.c err.h
