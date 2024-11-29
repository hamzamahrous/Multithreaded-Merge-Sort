# Multithreaded Merge Sort

This repository contains an implementation of the Merge Sort algorithm in C++ with a focus on multithreading to enhance performance. The project includes:

- **Sequential Merge Sort**: A standard recursive implementation.
- **Multithreaded Merge Sort**: A parallel implementation that dynamically creates threads based on the number of available CPU cores or user-specified limits.
- **Performance Measurement**: Compares the execution time of sequential and multithreaded versions for large datasets.

## Key Features

- Written in modern C++ using `std::thread` and `std::chrono`.
- Dynamically adjusts the number of threads based on hardware concurrency or user input.
- Includes a base case for small subarrays to avoid unnecessary thread overhead.
- Demonstrates practical use of multithreading and recursion in sorting algorithms.
