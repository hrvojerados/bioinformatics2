CXX = g++
CXXFLAGS = -O3 -march=native -mtune=native -flto -funroll-loops -fomit-frame-pointer -fno-semantic-interposition

TEST_DIR = tests

TEST_SRCS = $(wildcard $(TEST_DIR)/test*.cpp)
TEST_BINS = $(patsubst %.cpp, %, $(TEST_SRCS))

all: $(TEST_BINS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TEST_BINS)

.PHONY: all clean
