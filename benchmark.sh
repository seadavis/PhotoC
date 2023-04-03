perf record -F 99 -a -g -- ./bin/benchmarks --benchmark_filter=BM_CompositeCalculations/500x405
perf report