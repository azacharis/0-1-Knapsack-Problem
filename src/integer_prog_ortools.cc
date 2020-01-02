#include "../ortools/include/ortools/linear_solver/linear_solver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>
namespace operations_research{
std::vector<int> KnapsackInteger(std::vector<int64> &values,std::vector<int64> &weights, int capacity){
    MPSolver solver("IP KNAPSACK SOLVER", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    MPConstraint *const c = solver.MakeRowConstraint(0, capacity, "capacity_constraint");
    MPObjective *const objective = solver.MutableObjective();
    std::vector<MPVariable*> KnapVar;                                                  
    for (int i=0;i<values.size();i++){
      std::string s= "x" + std:: to_string((i+1));
      MPVariable *const x = solver.MakeBoolVar(s);
      KnapVar.push_back(x);
       c->SetCoefficient(x, weights.at(i));
       objective->SetCoefficient(x, values.at(i));
    }
    objective->SetMaximization();
    solver.Solve();
   
    
    // Solution
    std::vector<int> packed_items;
    for (int i=0;i<KnapVar.size();i++){
       int solValue=KnapVar.at(i)->solution_value();
       if (solValue==1) packed_items.push_back(i);
    }
    return packed_items;
}
} // namespace operations_research


// main
 int main(int argc, char *argv[]){
  if (argc!=3)  {
      std::cout << "No correct arguments \n";
	    exit(0);
	}
  std::cout << "Integer Programming Solver by ortools\n";
  char* inputFilename = argv[1];
  char* outputFilename = argv[2];
  std::cout <<"Input file: "<< inputFilename<<"\n";
  std::cout <<"Output file: "<< outputFilename<<"\n";

   std::ifstream fin(inputFilename);  
   if (!fin.good()) {    
        std::cerr << "Error opening file " << inputFilename << std::endl; 
        system("pause");
        exit(-1);  
    }

    int items_number;
    fin >> items_number; 
    std::cout<<"items_number= "<<items_number<<"\n";
    std::vector<int64> values;
    std::vector<int64> weights;
    int id;    
    for (int i = 0; i < items_number; i++){
        fin >> id;  
        int v;
        fin >> v;
        values.push_back(v);
        int w;
        fin >> w;
        weights.push_back(w);
    }
    int capacity;
    fin >> capacity;   
    std::cout<<capacity<<"\n";
    
    auto begin = std::chrono::high_resolution_clock::now();
    
    std::vector<int> solved_items = operations_research::KnapsackInteger(values,weights,capacity);
    
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
    unsigned long last_sol_time = duration.count()/1000.0;
    
    int max_weight=0;
    int max_profit=0;
    std::cout<<"Item of solution\n";
    for (int i=0;i<solved_items.size();i++){
      int selectedItem = solved_items.at(i);
      int current_weight = weights.at(selectedItem);
      int current_profit = values.at(selectedItem);
      std::cout<<current_profit<<" " <<current_weight <<"\n";
      max_profit+=current_profit;
      max_weight+=current_weight;
      
    }
    std::cout<<"max_weight="<<max_weight<<"\n";
    std::cout<<"max_profit="<<max_profit<<"\n";
    std::cout<<"last_sol_time="<<last_sol_time<<"\n";
    
    
    std::fstream fout;
    fout.open(outputFilename, std::ios::app);
    if (!fout.good()) {     
        std::cerr << "--- Error opening file " << outputFilename << std::endl;
        exit(-1);      
    }
    
      fout <<";"<<max_weight<<";"<<max_profit<<";"<<last_sol_time;
    
      fout.close();
  
  return EXIT_SUCCESS;
}


