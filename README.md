# Spektral Threads - The Fastest CUDA Threading Utility

## Introduction

Spektral Threads is a light-weight utility for assigning jobs to threads with a
focus on minimizing cache trampling. Spektral implements predictive pooling
through intelligent captures and granting each thread exclusive memory access.

## Benchmarks

### Testing Methodology


1. Run RegEx matching program on a ``100MB`` randomly generated file 
2. Repeat 500 times
3. Report average time

| Tool | Avg. Time | Time multiple |
|------|------|---------------|
|``Spektral::Threads``|13 us|1.00x|
|``BS::thread_pool``|17 us |1.31x|
|``std::async``|70 us|5.38x|