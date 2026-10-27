CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -O0 -g -fno-omit-frame-pointer
LDFLAGS := -no-pie
TARGET := test_hw79

.PHONY: all test check-source test-p1 test-p2 test-p3 test-p4 test-p5 test-p6 clean

all: $(TARGET)

$(TARGET): hw79.s tests/helpers.s tests/test_hw79.c hw79.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. hw79.s tests/helpers.s tests/test_hw79.c -o $@

check-source:
	python3 tests/check_source.py

test: check-source $(TARGET)
	./$(TARGET)

test-p1: $(TARGET)
	./$(TARGET) p1

test-p2: $(TARGET)
	./$(TARGET) p2

test-p3: $(TARGET)
	./$(TARGET) p3

test-p4: $(TARGET)
	./$(TARGET) p4

test-p5: $(TARGET)
	./$(TARGET) p5

test-p6: $(TARGET)
	./$(TARGET) p6

clean:
	rm -f $(TARGET) *.o tests/*.o
