CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra

RM= rm -f

INET_NTOA_T= inet_ntoa
INET_NTOA_O= inet_ntoa.o

# Targets start here
t: $(INET_NTOA_T)

$(INET_NTOA_T): $(INET_NTOA_O)
	$(CC) -o $@ $(INET_NTOA_O)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(INET_NTOA_T) $(INET_NTOA_O)

.PHONY: t echo clean

inet_ntoa.o: inet_ntoa.c
