#!/bin/bash
#PBS -l select=1:ncpus=1:epp=true:mem=100mb,walltime=01:00:00
#PBS -V
#PBS -j oe

cd /home/epp/phraar/Documents/Programs/Voxels/LArVox/0.1
./LArVoxTest_HP -m 20 -R /tmp/genie_qel.root /home/epp/phraar/Documents/Programs/Voxels/events/genie-770MeVQELCC.root
mv /tmp/genie_qel.dat  /home/epp/phraar/

