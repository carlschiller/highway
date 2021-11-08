# Highway
Simulating traffic flow at Roslags Näsby trafikplats in Täby, Sweden as a part of KTH course work in SI1336. Model of road is constructed by using a directed graph with vertices spaced approximately every 30
meters. Cars are simulated by traversing the graph with a time step size of 1/60th of a second.
 
![](/latex/pic2.png =500x600)

## Installation on Linux
A Dockerfile is provided to automatically compile the code and it's dependencies. To use the GUI, some Xauthority configuration is needed to be performed. A safe manner is provided by running the shell script provided

```
chmod +x build_and_run_image.sh
sudo ./build_and_run_image.sh
```

## Features
- 