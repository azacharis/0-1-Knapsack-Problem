1. The knapsack_gen.exe file creates a knapsack problem.
Τhe user put the 5 parameters and the generator stores the problem in a                                                                 text file [problem_(parameter1)_(parameter2)_(parameter3)_(parameter4)_(parameter5).txt]
The parameters n: number of items(10,50,100,500), r: range of coefficients(50,100,500,1000), type: 1=uncorr., 2=weakly corr., 3=strongly corr., 4=subset(1,2,3,4) sum i: instance no(1,2,3,4,5) S: number of tests in series (typically 5)
2.The fileCreator file creates all the necessary knapsack files for exercise by calling the gen.exe generator 320 times.
For example fileCreator gen.exe problem_.txt
Creates files in the same folder with names:
problem_10_50_1_1_5.txt
problem_10_50_1_2_5.txt
problem_10_50_1_3_5.txt
problem_10_50_1_4_5.txt
problem_10_50_1_5_5.txt
.
.
.
.
.
.
problem_500_1000_4_5_5.txt
