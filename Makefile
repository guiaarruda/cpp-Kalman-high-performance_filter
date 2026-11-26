CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRC      = src/core/Matrix.cpp src/physics/Particle.cpp src/physics/KalmanFilter.cpp

all: kalman test_matrix_assert monte_carlo bench

kalman: tests/test_kalman.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

test_matrix_assert: tests/test_matrix_assert.cpp $(SRC)
	$(CXX) -std=c++17 -O0 -g -Wall $^ -o $@

monte_carlo: benchmarks/monte_carlo.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

bench: benchmarks/bench.cpp $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: test_matrix_assert
	./test_matrix_assert

clean:
	rm -f kalman test_matrix_assert monte_carlo bench

.PHONY: all test clean