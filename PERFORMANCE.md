# Performance

The primary goal of this project is to improve rendering performance through parallelization. This document discusses the performance gains achieved.

## Sequential vs. Parallel

A sequential ray tracer processes each pixel one by one. On a multi-core processor, this leaves most of the CPU cores idle.

By parallelizing the main rendering loop with OpenMP, we can utilize all available cores. The `#pragma omp parallel for` directive splits the iterations of the pixel loop among multiple threads, allowing many pixels to be processed concurrently.

## Measuring Performance

The execution time of the `render()` function is measured to quantify the performance improvement. The speedup can be calculated as:

