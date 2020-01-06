CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= ..
FILE= data.txt

MYCFLAGS= -I$(ROOT)
MYLDFLAGS= -L$(ROOT)/common
MYLIBS= -lerr

LOW_OPEN_T= low_open
LOW_OPEN_O= low_open.o

# Targets start here
t: $(LOW_OPEN_T)

$(LOW_OPEN_T): $(LOW_OPEN_O)
	$(CC) -o $@ $(LDFLAGS) $(LOW_OPEN_O) $(LIBS)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(LOW_OPEN_T) $(LOW_OPEN_O) $(FILE)

.PHONY: t echo clean

low_open.o: low_open.c
