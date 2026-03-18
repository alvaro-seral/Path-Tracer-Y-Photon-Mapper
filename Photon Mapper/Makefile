CXX = g++

CXXFLAGS = -Wall -Wextra -Wconversion -pedantic $(INCLUDE) -march=native --std=c++17

LDFLAGS = -pthread
LDLIBS =

SRC = src
BUILD = build
TESTS = tests

MODULES = file geometry image illumination math_structures math_transformations materials primitives render utils photon

SRCS = $(foreach dir, $(MODULES), $(wildcard $(SRC)/$(dir)/*.cc))
OBJS = $(patsubst $(SRC)/%.cc, $(BUILD)/%.o, $(SRCS))
INCLUDE = -I./$(SRC) $(foreach dir, $(MODULES), -I./$(SRC)/$(dir))

TARGET = photon_mapper

all: $(TARGET)

$(TARGET): $(OBJS) $(BUILD)/main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(BUILD)/%.o: $(SRC)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


release: CXXFLAGS += -O2 -DNDEBUG
release: $(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

run: $(TARGET)
	./$(TARGET)

docs:
	doxygen Doxyfile

clean_docs:
	$(RM) -r docs/*

clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) -r $(BUILD)/*

.PHONY: all clean clean_docs docs run