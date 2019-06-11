# Normalizer
Multi-Threading project of the Operating Systems course.
An application that normalize data of sensors in the each time, using multi thread programming.  
Control threads and syncing them and checking correctness of answer handled by semaphores.

### Running
Run the below codes in terminal
```
g++ sensor.cpp -pthread -o sensor && ./sensor
```

Normalized data of inputs.txt file will be store in result.txt file.
