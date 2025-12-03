CXX = g++
CXXFLAGS = -O2 -g -std=c++20 -Wall -Wextra -I../containers
TARGET = profile

all: $(TARGET)

$(TARGET): profile.cpp
	$(CXX) $(CXXFLAGS) profile.cpp -o $(TARGET)

perf: $(TARGET)
	perf record -g ./$(TARGET)
	perf report

stat: $(TARGET)
	perf stat -d ./$(TARGET)

cachegrind: $(TARGET)
	valgrind --tool=cachegrind ./$(TARGET)

clean:
	rm -f $(TARGET) cachegrind.out.* callgrind.out.* perf.data perf.data.old

.PHONY: all perf stat cachegrind callgrind view-cache view-call clean

