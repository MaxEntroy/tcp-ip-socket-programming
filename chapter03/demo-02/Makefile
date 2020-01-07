CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra

RM= rm -f

INET_ADDR_T= inet_addr
INET_ADDR_O= inet_addr.o

# Targets start here
t: $(INET_ADDR_T)

$(INET_ADDR_T): $(INET_ADDR_O)
	$(CC) -o $@ $(INET_ADDR_O)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(INET_ADDR_T) $(INET_ADDR_O)

.PHONY: t echo clean

inet_addr.o: inet_addr.c
