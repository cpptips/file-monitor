# 自动检测编译器
ifeq ($(OS),Windows_NT)
    CXX ?= g++
    PLATFORM := Windows
    LDFLAGS += -lpsapi
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CXX ?= /opt/compiler/gcc-12/bin/g++
        PLATFORM := Linux
        LDFLAGS += -lstdc++fs
    endif
    ifeq ($(UNAME_S),Darwin)
        CXX ?= clang++
        PLATFORM := macOS
        # macOS上C++17 filesystem库不需要额外链接
    endif
endif

# 公共编译选项
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
TARGET = file_monitor
CONFIG_TARGET = file_monitor_config
SRC = main_fixed.cpp
CONFIG_SRC = main_with_config.cpp

all: $(TARGET) $(CONFIG_TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(CONFIG_TARGET): $(CONFIG_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(CONFIG_TARGET)

run: $(TARGET)
	./$(TARGET)

run_config: $(CONFIG_TARGET)
	./$(CONFIG_TARGET) config.json

.PHONY: all clean run run_config