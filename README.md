# Normalizer
Multi-Threading project of the Operating Systems course.
An application that normalize data of sensors in the each time, using multi thread programming.  
Control threads to avoid from race condition and checking correctness of answer handled by semaphores.

### Running
Run the below command in terminal
```
g++ sensor.cpp -pthread -o sensor && ./sensor
```

Normalized data of inputs.txt file will be store in result.txt file.
