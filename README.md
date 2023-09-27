# Exception Research

|Столбец 1|Столбец 2|Столбец 3|
|-|--------|---|
|Длинная запись в первом столбце|Запись в столбце 2|Запись в столбце 3|
|Кртк зпс| |Слева нет записи|

## Experiments

### OpenMP
- `parallel [num_threads]; critical` [here](./experiments/n1/main.cpp)
- `parallel [num_threads]; critical; nested` [here](./experiments/n2/main.cpp)
- `one thread - one file` [here](./experiments/n3/main.cpp)