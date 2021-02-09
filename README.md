## Build

To build, run 

```
mkdir build && cd build && cmake ..
make
```

## Run 

This program takes exactly one argument, which is the path to a (valid) ini file. Run from 
bin-directory. E.g. 

```
cd bin
./AlphabetConverter ../Init/example.ini
```

## Data

The example.txt data in the ./Data directory has been taken from the CTU-13 dataset. For more 
information, pleaser refer to "An empirical comparison of botnet detection methods" Sebastian Garcia, 
Martin Grill, Jan Stiborek and Alejandro Zunino. Computers and Security Journal, 
Elsevier. 2014. Vol 45, pp 100-123. http://dx.doi.org/10.1016/j.cose.2014.05.011 