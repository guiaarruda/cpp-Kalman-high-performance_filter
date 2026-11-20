CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
BUILD    := build

CORE    := src/core/Matrix.cpp
PHYSICS := src/physics/Particle.cpp src/physics/KalmanFilter.cpp

.PHONY: all test bench run plot clean

all: $(BUILD)/kalman $(BUILD)/test_matrix $(BUILD)/test_monte_carlo $(BUILD)/bench

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/kalman: src/main.cpp $(CORE) $(PHYSICS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test_matrix: tests/test_matrix.cpp $(CORE) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test_monte_carlo: tests/test_monte_carlo.cpp $(CORE) $(PHYSICS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/test_physics: tests/test_physics.cpp $(CORE) src/physics/Particle.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/bench: benchmarks/bench.cpp $(CORE) $(PHYSICS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(BUILD)/kalman
	./$(BUILD)/kalman

test: $(BUILD)/test_matrix $(BUILD)/test_monte_carlo
	./$(BUILD)/test_matrix
	./$(BUILD)/test_monte_carlo

bench: $(BUILD)/bench
	./$(BUILD)/bench

plot: run
	python3 graphs.py

clean:
	rm -rf $(BUILD) results.csv
