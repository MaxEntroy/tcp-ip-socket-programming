CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra

RM= rm -f

INET_ATON_T= inet_aton
INET_ATON_O= inet_aton.o

# Targets start here
t: $(INET_ATON_T)

$(INET_ATON_T): $(INET_ATON_O)
	$(CC) -o $@ $(INET_ATON_O)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(INET_ATON_T) $(INET_ATON_O)

.PHONY: t echo clean

inet_aton.o: inet_aton.c
