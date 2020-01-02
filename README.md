# 0-1-Knapsack-Problem
This repository contains 6 solvers about the knapsack 0-1 problem
This repository include C++ codes and execute files to solve this problem

The generator folder contains two cpp files and two execute files to create one knapsack 0-1 problem or a pack of problems

The problems can solved using 6 algorithms:

1. **Greedy approach**
2. **Brute force**
3. **Branch and bound**
4. **Dynamic programming**
5. **Dynamic OR-Tools solver**
6. **Integer OR-Tools solver**

The solution of each problem is stored in a file

The solution of pack of problem is stored in a CSV file for generating statistics such as total_value,total_weight and execution time 

## Instructions:
1. Download this repository
2. Run the file κnapsack.exe from the command line
3. On the menu select the algorithm with which you want to solve all problems or one of then
You can create a CSV files with the solutions of all problems with 6 algorithms
4. knapsack.exe This is the main executable file. Contains the solvers 
5. dynamic_prog_ortools.exe Util that used by the main file to solve a problem using the OR-Tools Dynamic algorithm
6. integer_prog_ortools.exe Util that used by the main file to solve a problem using the OR-Tools Integer algorithm
7. knapsack_gen.exe Util that used by the main file to create a knapsack problem,you can find more instructions about this in the text file included in the generator folder.

**P.S. All of these files must be in the same folder.** 

**You can visit my github website at <a href="https://azacharis.github.io/0-1-Knapsack-Problem"/>click here</a>**
