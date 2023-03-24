# Round Robin CPU Scheduling Algorithm
Various implementations of Round Robin Scheduling Algorithm

This project is done as part of a University Module.

## Overview

This project aims to conduct analysis on study on the various Round Robin scheduling algorithms, these algorithms include:

- Round Robin
- [Optimal Round Robin CPU Scheduling Algorithm using Manhattan Distance](https://www.researchgate.net/publication/320855939_Optimal_Round_Robin_CPU_Scheduling_Algorithm_Using_Manhattan_Distance)
- [Best Time Quantum Round Robin CPU Scheduling Algorithm](https://ijseas.com/volume3/v3i5/ijseas20170529.pdf)
- [Yosef Round Robin](https://ieeexplore.ieee.org/document/8716476)
- [Sorted Round Robin](https://ieeexplore.ieee.org/document/8862609)

## How to

Compile all the C source and header files in `RoundRobinAlgorithm` folder, then run it. Follow the prompts in the console commands to selected desired Round Robin simulator, and the inputs you wish to give.
> A make file has been provided for easier compiling.

For every round robin simulated, a data file will be generated into a folder, `analysis_data`. The data file can be passed to the analyzer project to create a graph.

## Analyzer

Analyzer is a Python project built to create graphs out of the data from the simulated Round Robin. It is located in `RoundRobinAnalyzer`, run `main.py`.

Most of the values and configurations are slightly hard-coded. Edits to the code to display different graphs can be done in `main.py`.
