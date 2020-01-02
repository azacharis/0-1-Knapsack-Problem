1. The knapsack_gen.exe file creates a knapsack problem
accepts 5 parameters and stores the problem in a file problem_(parameter1)_(parameter2)_(parameter3)_(parameter4)_(parameter5).txt

2.The fileCreator file creates all the necessary knapsack files for exercise by calling the gen.exe generator 320 times.
For example fileCreator gen.exe problem_.txt
Creates files in the same folder with names:
problem_10_50_1_1_5.txt
problem_10_50_1_2_5.txt
.
.
.
.
.
.
problem_500_1000_4_5_5.txt
