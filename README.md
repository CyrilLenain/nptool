NPTOOL
======

The NPTool package is based on Root and Geant4. It is aimed to assist experimental nuclear physicist is performing complex data analysis and simulations. NPTool philosophy is to provide an open framework for the community, hence license under GPL2. If you wish to contribute, contact Adrien MATTA at a.matta@surrey.ac.uk

#Getting the code
### Using git
The recommended method to obtain the code is to use git. This insure an easy way to access the update version of the code. First make sure you got git installed, if not, just use your package manager to get it. Then go to the directory where you want to install the nptool folder and type:
````
$ git clone https://github.com/adrien-matta/nptool
````
This will create the nptool folder with the latest version of nptool.

### Downloading from Git Hub
Alternatively you can browse the following page https://github.com/adrien-matta/nptool, and click the Download ZIP button on the right side of the page. Simply unzip the archive at the desire location

#Setup

###Requirements
In order to compile the core libraries NPLib, you will need ROOT 6 or 5 (tested with 5.34) to be installed with the libMathMore.so library. This is sufficient to compile NPLib and any analysis project. In order to compile NPSimulation, you will need a recent installation of Geant4 (tesetd with version 9.6 and 10.1).

###Building NPLib
NPLib is the core of the NPTool package, holding most of the actual code. It is made of a collection of stand alone C++ classes that can be used in programs and macro. The first step is to define the variable of environment that the system needs. For this open your .profile / .bashrc / .tcshrc file in your home directory and add the following line:

> source /path/to/nptool/nptool.sh

Restart your terminal. You should now have all aliases and environment variable properly defined and can now access the NPLib folder by using the command:
````
$ npl 
````
and you should be in the NPLib directory. 
Before the compilation of the libraries you need to specify the detector you plan to use by calling the configure script. If you give no argument to configure, all detector will be compile. If you wish to limit the number of detector to be compiled, simply specify the detector folder name (respecting the case). You can specify more than one detector:

````
$ cmake ./ 
````
OR:
````
$ cmake ./ -DETLIST="DetFolder1 DetFolder2"
````

Then you compile the whole NPLib with n threads using :

````
$ make -jn install
````

If you wish to recompile with more detector:

````
$ rm CMakeCache.txt
$ cmake ./ -DETLIST="DetFolder1 DetFolder2 ..."
$ make -jn install
````

If you have google ninja build install then you can alternatively ask cmake to generate the ninja.build file:
````
$ cmake -GNinja ./
$ ninja install
````

Compilation using Ninja is usually twice faster than using make

###Building NPSimulation
This part of the package rely on Geant4 to perform Monte Carlo simulation. You need to first compile and configure correctly NPLib in order for NPSimulation to compile and run. The compilation is done as follow:
````
$ nps
$ cmake ./
$ make -jn install
`````

This will produce the npsimulation executable. For a detail list of the available input flag and their meaning, you can run:
````
$ npsimulation -h
````

To run a standardised test case, you can run the following command:
````
$ npsimulation -D Example1.detector -E Example1.reaction -O Example1
````
This will open the NPSimulation GUI (if you are using Qt) or the prompt terminal. In either case you can generate event using:
````
> run/beamOn/ 10000
> exit
````

This will run the 11Li(d,3He)10He->8He+n+n simulation and produce a root file located in $NPTOOL/Outputs/Simulation/Example1.root. One can have a look at the Example1.detector, located in $NPTOOL/Inputs/DetectorConfiguration, and Example1.reaction, located in $NPTOOL/Inputs/EventGenerator, to see how the input file are formated. They usually are self explenatory using easy to understand token.

You can now try to analyse this simulated tree using the associated NPAnalysis project:
````
$ npa
$ cd Example1/
$ cmake ./
$ make -jn
$ npanalysis -R RunToTreat.txt -O Example1
````

Because the input file are written in the simulation file along the SimulatedTree, npanalysis will automatically use those file as inputs. This will produce the analysed tree located in $NPTOOL/Outputs/Analysis/Example1.root. You can then display the result of the simulation using root:
````
$ root ShowResult.C
````

You should be able to see the light particle identification, the light particle kinematical line and the associated excitation energy spectrum fitted by a gaussian.
 
The Example1 input files and NPAnalysis project are simple basis that can be used to start doing your own simulations.

