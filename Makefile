CC := gcc
CSTD := -std=c11
WARN := -Wall -Wextra -Wpedantic
CFLAGS := $(CSTD) $(WARN) -Iinclude
BUILD_DIR := build

TARGET_SJF := sjf_simulador
TARGET_RR := roundRobin_simulador

.PHONY: all clean

all: $(TARGET_SJF) $(TARGET_RR)

$(TARGET_SJF): $(BUILD_DIR)/sjf_simulador.o
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_RR): $(BUILD_DIR)/roundRobin_simulador.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_SJF) $(TARGET_RR)