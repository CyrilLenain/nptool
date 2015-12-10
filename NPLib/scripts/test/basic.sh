#!/bin/bash

# exit on error, and verbose
set -ev

# can run the app
npanalysis -h

# Compile Example1 
cd $NPTOOL/NPAnalysis/Example1
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi
# Moving reference test simulation
cp $NPTOOL/Benchmarks/Example1_Simu.root $NPTOOL/Outputs/Simulation/Example1.root
npanalysis -R RunToTreat.txt -O Example1

echo ".q" | root -b -l ShowResult.cxx

# Compile Example2
cd $NPTOOL/NPAnalysis/Example2
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi
cp $NPTOOL/Benchmarks/Example2_Simu.root $NPTOOL/Outputs/Simulation/Example2.root
npanalysis -R RunToTreat.txt -O Example2
echo ".q" | root -b -l ShowResult.cxx
