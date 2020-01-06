CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

RM= rm -f

ROOT= ..
FILE= data.txt
FILE1= test.dat

MYCFLAGS= -I$(ROOT)
MYLDFLAGS= -L$(ROOT)/common
MYLIBS= -lerr

LOW_OPEN_T= low_open
LOW_OPEN_O= low_open.o

FD_SERI_T= fd_seri
FD_SERI_O= fd_seri.o

# Targets start here
t: $(LOW_OPEN_T)

t1: $(FD_SERI_T)

$(LOW_OPEN_T): $(LOW_OPEN_O)
	$(CC) -o $@ $(LDFLAGS) $(LOW_OPEN_O) $(LIBS)

$(FD_SERI_T): $(FD_SERI_O)
	$(CC) -o $@ $(FD_SERI_O)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(LOW_OPEN_T) $(LOW_OPEN_O) $(FILE) $(FD_SERI_T) $(FD_SERI_O) $(FILE1)

.PHONY: t t1 echo clean

low_open.o: low_open.c
fd_seri.o: fd_seri.c
