#include "../ortools/include/ortools/algorithms/knapsack_solver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>


namespace operations_research{
std::vector<int> KnapsackExample(std::vector<int64> &values,std::vector<std::vector<int64>> &weights, std::vector<int64> &capacities ){
    KnapsackSolver solver(KnapsackSolver::KNAPSACK_DYNAMIC_PROGRAMMING_SOLVER, "SimpleKnapsackExample");
    solver.Init(values, weights, capacities);
    int64 calculate_value = solver.Solve();
    std::cout << "Total value: " << calculate_value << std::endl;

    std::vector<int> items;
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        if (solver.BestSolutionContains(i)) {
            items.push_back(i);
            }
    }
    return items;
}
} // namespace operations_research


//main


 int main(int argc, char *argv[]){
  if (argc!=3)  {
      std::cout << "No correct arguments \n";
	    exit(0);
	}
  
  std::cout << "Dynamic Programming Solver ortools\n";
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
    
 
    
    std::vector<std::vector<int64>> current_weights_vector;
    current_weights_vector.push_back(weights);
    std::vector<int64> kcapacity;
    kcapacity.push_back(capacity);

    auto begin = std::chrono::high_resolution_clock::now();
    
    std::vector<int> solved_items = operations_research::KnapsackExample(values,current_weights_vector,kcapacity);
    
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
      std::cout<<current_profit<<" " << current_weight <<"\n";
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
