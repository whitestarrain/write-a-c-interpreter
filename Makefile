.PHONY: all clean test

BIN=output

BINS = xc calculate
LIST = $(addprefix $(BIN)/, $(BINS))

all: $(LIST)

$(BIN)/xc: CFLAGS := -g -m32
$(BIN)/calculate: CFLAGS := -g

$(BIN)/%: %.c
	-mkdir -p $(BIN)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -rf output

