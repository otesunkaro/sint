CC ?= cc
CFLAGS ?= -Wall -Wextra -O2
CFLAGS += -Iinclude -MMD -MP -lm
TARGET := sint

SRC := $(shell find src -name '*.c')
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(LDLIBS) $(LDFLAGS) $^

build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -o $@ -c $(CFLAGS) $<

clean:
	rm -rf ./build

fmt:
	find src include -name '*.[ch]' -exec clang-format --style=llvm -i {} +

-include $(OBJ:.o=.d)

.PHONY: all clean fmt
