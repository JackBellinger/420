#!/bin/zsh -l

# File: pass_crack_cluster.sh

#SBATCH --job-name=jackson-sean-pass-crack
#SBATCH --nodes=10
#SBATCH --ntasks-per-node=10
#SBATCH --mem=8gb
#SBATCH --time=1000:00:00    # Time limit in the form hh:mm:ss
#SBATCH --output=out/%j.log
#SBATCH --exclude=hpcl5-1


module load mpi/mpich-3.2-x86_64

# For a python3 project
# srun python3 ~/Projects/myproject/myproject.py

# For a C MPI project
time mpirun ./decrypt
