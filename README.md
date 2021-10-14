# PhysiCell_GPU: an OpenACC Acclerated Version of PhysiCell, an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

In this code, we carefully utilize OpenACC, adirective-based programming model to accelerate the diffusion portion in Equation 1 inPhysiCell, a cross-platform  agent-based biosimulationframework that has been adopted in cancer infectious diseases and othercomplex biological problem. 

Compile uses NVHPC, but you can change it in the Makefile. To build, just type `make`. The default is using Cuda managed memory.

Original: 
https://github.com/MathCancer/PhysiCell
 

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 


