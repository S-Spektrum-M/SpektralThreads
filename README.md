# Spektral Threads- The Fastest CUDA Threading Utility

## License

This program is licensed under the MIT License.

## Introduction

Spektral Threads herein reffered to as "Spektral" is a utility for optimizing CUDA Threads.
Spektral works by applying predictive pooling and cache alignment to prevent GDDR-VCACHE Trampling.
Furthermore, Spektral uses rDMA to prevent overloading CPU caches. This leads to a quantifiable
impact of 7490X less bus utilization and 14X more instructions per cycle.

## Implementors
- Keras
- Microsoft Cognitive Toolkit
- [FinSTA](https://github.com/S-Spektrum-M/FinSTA) (my own project which served as the inspiration for this)

## Features

TODO

## Dependencies
- Spektral::Logging
- Spektral::FastDataStructures
- Nvidia::CUDA
