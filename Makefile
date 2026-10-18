CXX    := g++
TARGET := game
BUILD  := build

# Every .cpp in this directory is compiled automatically
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

# -MMD -MP generates .d files so editing a .hpp rebuilds the .cpp files that include it
CXXFLAGS := -std=c++17 -Wall -Wextra -g -MMD -MP

# If raylib isn't installed system-wide, uncomment and point these at it:
# CXXFLAGS += -I/path/to/raylib/src
# LDFLAGS  += -L/path/to/raylib/src

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
  LDLIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(UNAME),Darwin)
  LDLIBS := -lraylib -framework IOKit -framework Cocoa -framework OpenGL
else
  # Windows (MinGW)
  LDLIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEPS)
