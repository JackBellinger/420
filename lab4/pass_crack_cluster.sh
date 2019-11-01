#!/bin/zsh -l

# File: pass_crack_cluster.sh

#SBATCH --job-name=jackson-sean-pass-crack
#SBATCH --nodes=10
#SBATCH --ntasks-per-node=12
##SBATCH --mem=8gb
#SBATCH --time=24:00:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log


module load mpi/mpich-3.2-x86_64

# For a python3 project
# srun python3 ~/Projects/myproject/myproject.py

# For a C MPI project
time mpirun ./decrypt
