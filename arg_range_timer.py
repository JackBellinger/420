import os
import subprocess
import argparse
import time
import matplotlib.pyplot as plt

#this script allows a pre-existing executable file to be run with specified args
#the executable must take a single integer argument

parser = argparse.ArgumentParser(description='Run and time an executable repeatedly over a range of arguments')
parser.add_argument('-e', '--exe-name', type=str, dest='exe',
        help='the range of arguments that the executable should be called with')
parser.add_argument('-r1', '--range1', type=int, nargs=2, dest='range1',
        help='the range of arguments that the executable should be called with')
parser.add_argument('-s', '--step-size', type=int, default=1, dest='step_size',
        help='the step size of the range')
parser.add_argument('-o', '--step-op', type=str, choices=['+', '*', '^'], default="+", dest='step_op',
        help='the operation to apply to the arg ie += step_size or *= step size')
parser.add_argument('-p', '--procs', type=int, nargs=2 dest='nprocs', default=[2,4],
        help='the number of processors to use for mpi')

args = parser.parse_args()

#params to pass to the executable

arg_seq = []

i = args.range1[0]
while i < args.range1[1]:
    arg_seq.append(i)
    if args.step_op == '+':
        i += args.step_size
    if args.step_op == '*':
        i *= args.step_size
    if args.step_op == '^':
        i = i ** args.step_size
print(arg_seq)

all_times = []
procs = [i for i in range(args.nprocs[0], args.nprocs[1])]
for n in procs:
    times = []
    for arg in arg_seq:
        start = time.time()
        command_line = ['mpiexec', '-n', str(args.nprocs), './'+args.exe, str(arg)]
        p = subprocess.Popen(command_line).wait()#call the executable synchronously
        end = time.time()
        times.append(end - start)
    all_times.append(times)
    plt.plot(arg_seq, all_times, 'ro', label=f"{n} Nodes")

print(times)

for i in procs:

plt.ylabel('Execution time')
plt.xlabel('Input argument')
#plt.axis([])
plt.show()
