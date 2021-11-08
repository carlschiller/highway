# Highway
Simulating traffic flow at Roslags Näsby trafikplats in Täby, Sweden as a part of KTH course work in SI1336. Model of road is constructed by using a directed graph with vertices spaced approximately every 30
meters. Cars are simulated by traversing the graph with a time step size of 1/60th of a second.
 
Built with a *custom* GUI using [SFML](https://github.com/SFML/SFML) in the C++11 standard. Multiprocessing of the simulation is enabled by default. 

<img src=latex/pic2.png width=300>

## Installation on Linux
A Dockerfile is provided to automatically compile the code and it's dependencies. To use the GUI, some Xauthority configuration is needed to be performed. A safe manner is provided by running the shell script provided

```
chmod +x build_and_run_image.sh
sudo ./build_and_run_image.sh
```

To run the program, enter the following command in the docker image
```
cd build && ./highway
```

## Features
- Easily visualize traffic from a birds eye view.
- Simulate in real time speed, or with a multiplier.
- Tune spawn rate of cars to your heart's content.
- Variable driver "aggressiveness".
- Ramp. Meter.
- Skip visualization and get the raw data written in a text file.
- And much more :)

## Dependencies
Application was built using the following dependencies

- [SFML](https://github.com/SFML/SFML) v2.4.2+dfsg-4
- [fonts-open-sans](https://packages.debian.org/sid/fonts-open-sans) v1.11-1

