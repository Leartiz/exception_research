# Exception Research

## Results
### Program *cmd* [here](./program/cmd/main.cpp)
- Results v0 [here](./measurements/cmd/v0/tables.md)
- Results v1 [here](./measurements/cmd/v1/tables.md)

### Program *cmd1* [here](./program/cmd/main.cpp)

## Experiments

### cpp

#### + OpenMP
- n1 - `parallel [num_threads]; critical` [here](./experiments/n1/main.cpp)
- n2 - `parallel [num_threads]; critical; nested` [here](./experiments/n2/main.cpp)
- n3 - one thread - one file [here](./experiments/n3/main.cpp)
- n4 - exception from thread [here](./experiments/n4/main.cpp)
- n5 - `omp_get_wtime` [here](./experiments/n5/main.cpp)
- n6 - `omp_get_wtime; omp_get_wtick` [here](./experiments/n6/main.cpp)
- n7 - `_OPENMP` [here](./experiments/n7/main.cpp)

#### std
- n8 - initializer list or constructor [here](./experiments/n8/main.cpp)
- n9 - vector/error creation time [here](./experiments/n9/main.cpp)
- n10 - code analysis with exceptions [here](./experiments/n10/main.cpp)
- n11 - vector/error throw time [here](./experiments/n11/main.cpp)

## Useful links
- Table generator for the site // IKSWEB / URL: https://iksweb.ru/tools/generator-table-html/
- Соглашения о вызовах (calling convention) // whowh0 / URL: https://habr.com/ru/articles/675774/
- Печальная правда о пропуске копий в C++ // Bright_Translate / URL: https://habr.com/ru/companies/ruvds/articles/551782/
- Zero cost exception handling vs setjmp/longjmp // stackoverflow / URL: https://stackoverflow.com/questions/4975504/zero-cost-exception-handling-vs-setjmp-longjmp
- Exception handling performance // janvorli / URL: https://github.com/dotnet/runtime/issues/77568?ysclid=lomh73ni1e5644794