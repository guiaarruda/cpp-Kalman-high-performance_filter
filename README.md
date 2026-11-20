# Kalman Filter Engine (C++)

A Kalman filter and the linear algebra underneath it, written from scratch in C++17 with no external libraries.
A small physics simulation generates noisy position measurements of a moving particle, and the filter reconstructs its trajectory.

![Kalman filter result](kalman_result.png)
*Kalman filter estimate (blue) tracking the true trajectory (green) from noisy sensor measurements (red).*

## Results

| Metric | Result |
|---|---|
| Position error reduction (RMSE) vs. raw measurements | **~61%** (0.50 → 0.20 m, 1,000 Monte Carlo runs) |
| Matrix multiplication speedup vs. first version | **~11–18×** (N = 64 to 512) |
| Filter step latency (predict + update, 4 states) | **~0.75 µs** (≈1.3 million steps/s) |

Numbers measured with `make test` and `make bench` (`g++ -O2`). Timings depend on the machine.

## Key Features

* **Linear algebra engine**
    * `Matrix` class stored in a single contiguous `std::vector<double>` (row-major), avoiding the scattered allocations of `vector<vector<double>>`.
    * Multiplication in **i-k-j loop order**: the inner loop walks rows of both operands contiguously, instead of striding down a column. This is the main source of the speedup above.
    * Inverse by **Gauss–Jordan elimination with partial pivoting**, multiplying by the pivot's reciprocal instead of dividing inside the inner loop.
* **Kalman filter**: prediction (time update) and correction (measurement update) with the optimal Kalman gain.
* **Simulation and evaluation**: a particle moving at constant velocity with Gaussian sensor noise, plus a Monte Carlo test that measures the filter's error over many independent runs.

## Project Structure

```
src/core/          Matrix class
src/physics/       KalmanFilter and Particle (simulation)
src/main.cpp       Tracking demo, writes results.csv
tests/             Unit tests (Matrix) and Monte Carlo evaluation
benchmarks/        Matrix multiplication and filter-step benchmarks
graphs.py          Plots results.csv
```

## Quick Start

Requirements: `g++` with C++17 and `make`. Python 3 is only needed for the plot.

```bash
make run       # build and run the tracking demo (writes results.csv)
make test      # unit tests + Monte Carlo evaluation
make bench     # benchmarks
```

To plot the result:

```bash
python3 -m venv venv && source venv/bin/activate
pip install -r requirements.txt
make plot      # writes kalman_result.png
```

## How It Works

1. **Prediction (time update):** projects the state forward with the constant-velocity model, `x = F x`, and grows the uncertainty, `P = F P Fᵀ + Q`.
2. **Correction (measurement update):** compares the prediction with the measurement `z`, computes the Kalman gain `K = P Hᵀ (H P Hᵀ + R)⁻¹` and fuses both, reducing the covariance `P`.

## Tuning Notes

`R` is set to the variance of the simulated sensor noise (σ² = 0.25), and `Q` is small (10⁻³) because the particle moves at constant velocity.
The Monte Carlo test compares this tuning with the original hand-picked one (Q = 0.1, R = 2.0): the error reduction goes from ~55% to ~61%.
