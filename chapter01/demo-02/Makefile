CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra

RM= rm -f

ENDIAN_CONV_T= endian_conv
ENDIAN_CONV_O= endian_conv.o

# Targets start here
t: $(ENDIAN_CONV_T)

$(ENDIAN_CONV_T): $(ENDIAN_CONV_O)
	$(CC) -o $@ $(ENDIAN_CONV_O)

echo:
	@echo "CC= $(CC)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(ENDIAN_CONV_T) $(ENDIAN_CONV_O)

.PHONY: t echo clean

endian_conv.o: endian_conv.c
