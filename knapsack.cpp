#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <algorithm>
#include <limits>
#include <queue>
using namespace std;
using namespace std::chrono;
// � struct item �������� �� �������� ���� ������������
struct item
{
    int id;         // ������� ������������ �� �� ����� ��� ��������� ��� ������
    int profit;     // ����
    int weight;     // �����
    int bound;      //�����
    int level;      //������� �������
    std::vector<int> itemList; //����� ��� ��� ������ ��� branch and bound
};
int time_out = 10;
// �������� ��� struct �� ����� �� ���������� �� ��� ����� �� �� ��������� ��������
// ��� ������������ ��� �� �������� ���������� ��� ��������.
// ��� ��� ��������� ��� � ����������� ������ �� ��������� ���� �������� ��� ��� ����� �������� ��� �����������
struct knapsack_problem
{
    std::vector<item> items;     // ����� �� �� �������� �����������
    int capacity;           // ������������ ��������
    int time_limit = 10;      // ������ �������� ��� ����������� �� ���������� ��  seconds
};
int last_sol_time = 0;
/*read_data

   � ��������� read_data ������� �� ��������� �� ����� ���� ������� �� �������� knapsack ���
   ���������� �� ����������� �� ����� (struct) knapsack_problem
 */
knapsack_problem read_data(std::string& fn) {
    std::ifstream fin(fn);     // ����������� �� ��������� �� ������
    if (!fin.good()) {     // ��������� �� ��� ���� ��������
        std::cerr << "Error opening file " << fn << std::endl;   // ����������� ������ ������ �� ���� ��� ���������
        system("pause");
        exit(-1);         // ���� ������� ��� ����
    }

    knapsack_problem ks;  // ������������ ��� ��������� ����� knapsack_problem
                          // �� ���� �� �����k������ �� �������� ��� ������� knapsack
    int items_number;     // �������� ��� ��������� items_number ��� �� ������������ ���
                          // ������ ��� ������������ ��� ���������� ��� ������
    fin >> items_number;  // ���������� ��� ��� ����� ������ ��� ������ ��� ������������ ��� ��� ������� ���� items_number
    for (int i = 0; i < items_number; i++)  // ��� ��� ����� �� ����������� ��� ������� �� �������� ��� �������� ������
                          // ��� �� ���������� �� �������� ���� ������������
    {
        item an_item;     // � ��������� ���� ������� ���� ���� �� �������� ���� ������������ ��� �� ������
        fin >> an_item.id;      // ���������� �� id (����� ��� ������������ ��� ������)
        fin >> an_item.profit;  // ���������� ��� ���� ��� ��� ������������ ���� an_item.profit
        fin >> an_item.weight;  // ���������� �� ����� ��� �� ������������ ��� an_item.weight
        ks.items.push_back(an_item);  // ��������� �� an_item ��� ����� ��� ������ ��� ks (knapsack_problem)
    }
    fin >> ks.capacity;         // ����� ���������� ��� ������������ ��� �������� ��� ��� ������������ ��� ks.capacity
    return ks;  // ������������ ��� ���� ����� knapsack_problem � ����� �������� ��� �� ��o����� ��� ������������� �����������
}

bool cmp(struct item a, struct item b)
{
    double r1 = (double)a.profit / a.weight;
    double r2 = (double)b.profit / b.weight;
    return r1 > r2;
}

/*1 Greedy Approach*/
std::vector <item> greedy_approach_solver(knapsack_problem& ks)
{
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "Greedy Approach Solver\n";
    std::vector<item> itemsHolder(ks.items);    // ������������ ��� ��������� ��� ������ ��� ������������ ���� �� ����� ���������� ���� ��� ��������� ��� �� ��� ������� � ����� ���� ������ ������
    sort(itemsHolder.begin(), itemsHolder.end(), cmp);
    int current_weight = 0;              // ������ �������� ����� ���� ��� ����������
    double finalprofit = 0.0;        // ������� ���������: �������� ����� ������������ ��� �������� / ������������
    std::vector<item> sol_list;
    // Looping through all Items
    for (int i = 0; i < itemsHolder.size(); i++) {
        item tempItem = itemsHolder.at(i);
        // If adding Item won't overflow, add it completely
        if (current_weight + tempItem.weight <= ks.capacity)
        {
            sol_list.push_back(tempItem);
            current_weight += tempItem.weight;
            finalprofit += tempItem.profit;
        }
        // If we can't add current Item, add fractional part of it
        else
        {
            int remain = ks.capacity - current_weight;
            finalprofit += tempItem.profit * ((double)remain / tempItem.weight);
            break;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
        last_sol_time = duration.count() / 1000.0;
        if (last_sol_time > ks.time_limit) {
            std::cout << "out of time: time>" << ks.time_limit << "\n";
            return sol_list;
        }
    }
    std::cout << "max_weight=" << current_weight << "\n";
    std::cout << "max_profit=" << finalprofit << "\n";
    std::cout << "time=" << last_sol_time << "\n";
    return sol_list;
}

/*2  �rute Force Solver get_profit */
int get_profit(knapsack_problem& ks, std::vector<item>& sol)
{
    int total_profit = 0;
    int total_weight = 0;
    for (item an_item : sol)
    {
        total_weight += an_item.weight;
        if (total_weight > ks.capacity)
            return -1;
        total_profit += an_item.profit;
    }
    return total_profit;
}
/*brute force solver*/
std::vector <item> brute_force_solver(knapsack_problem& ks) {
    std::cout << "Brute Force Solver\n";
    auto begin = std::chrono::high_resolution_clock::now();
    std::vector<item> result;
    int max_profit = -1;
    int n = ks.items.size();
    int total = 1 << n;
    if (n >= 31)  total = std::numeric_limits<int>::max();
    for (int i = 0; i < total; i++)
    {
        std::vector<item> sol;
        for (int j = 0; j < n; j++)
        {
            if ((i >> j) & 1)
                sol.push_back(ks.items[j]);
        }
        int profit = get_profit(ks, sol);
        if (profit > max_profit)
        {
            max_profit = profit;
            result = sol;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
        last_sol_time = duration.count()/1000.0;
        if (last_sol_time > 10) {
            std::cout << "out of time: time>" << ks.time_limit << "\n";
            return result;
        }
    }
    std::cout << "max_profit=" << max_profit << "\n";
    int max_weight = 0;
    for (auto current_item : result) max_weight += current_item.weight;
    std::cout << "max_weight=" << max_weight << "\n";
    std::cout << "time=" << last_sol_time << "\n";
    return result;
}

/*3 Branch and Bound Solver*/
int bound(item u, int n, int W, item arr[]) {
    if (u.weight >= W) return 0;
    int profit_bound = u.profit;
    int j = u.level + 1;
    int totweight = u.weight;
    while ((j < n) && (totweight + arr[j].weight <= W)) {
        totweight += arr[j].weight;
        profit_bound += arr[j].profit;
        j++;
    }
    if (j < n) profit_bound += (W - totweight) * arr[j].profit / arr[j].weight;
    return profit_bound;
}

std::vector <int>  branch_and_bound(int W, item arr[], int n) {
    std::sort(arr, arr + n, cmp);
    std::queue <item> Q;
    item u, v;
    auto begin = std::chrono::high_resolution_clock::now();
    u.level = -1;
    u.profit = u.weight = 0;
    Q.push(u);
    int max_profit = 0;
    int max_weight = 0;
    std::vector<int> selectedItems;

    while (!Q.empty()) {        // � ���� �������� �� ����������� ��� ����� �� ��������. ������ ��� ����� ����� �����������
        u = Q.front();          // ������� ��� u �� ����� ����������� ��� ��� ����
        Q.pop();                // ��� ���� �� ��������� ���� �� ������� ��������
        if (u.level == -1) v.level = 0;    // ��� ����� ���� ��� �� ���������� �� loop � u.level ����� -1 ��� ��� ������� 0
        if (u.level == n - 1) continue;    // ��� �������� ��� ��������� ����������� ������� �� ��� ���� �� ���������� �� ������� ��� �����
        v.level = u.level + 1;  // � ������� �� ����� ��� ������� level (�����������)
        v.weight = u.weight + arr[v.level].weight;    //������������ ��� v �� ������� ����������� ��� ����������� �� ����� ���� ����� ��� ����
        v.profit = u.profit + arr[v.level].profit;
        v.id = arr[v.level].id;
        v.itemList.clear();                           // ���� ������ �� �� ����� ����. �� �� ������ ��� �����
        v.itemList = u.itemList;
        v.itemList.push_back(v.id);
        if (v.weight <= W && v.profit > max_profit) {  // ��� �� ����� ��� ���� ��������� �� ������� ����� ��� � ���� ��� ��� ����� ��������� ��� ���� ��� ����� ���� ������
            max_profit = v.profit;                     // ���������� �� ����������� ��������� �� �� �������� ����� ���� ������� ��� �� ����� ��� ��� ����
            max_weight = v.weight;
            selectedItems = v.itemList;               // ������� ��� �� ����� �� �� ���������� ����������� ��� ���������� �� � �������� ����� ����
        }
        v.bound = bound(v, n, W, arr);                // ������������ ��� ������� ���� ��� ��� ����������� ����������
        if (v.bound > max_profit) {                    // �� � ������� ���� ����� �������� ��� ���� ��� ������
            Q.push(v);                                // �� ������ �� ������� � ������ ��� v
        }
        v.weight = u.weight;                          // ���� �� ��������� ���� �� ����������� ���� ���� ��� ������ ���� ���� ��� ���� ��� �� �������������� ��� ���� ��� ���� ��� �����
        v.profit = u.profit;                          // �� ������ ��� �� ������������� ��� �� ���������������� �� ������ �����������
        v.id = u.id;
        v.itemList = u.itemList;
        v.bound = bound(v, n, W, arr);                // ������������ ���� ��� ������� ������ ���� ��� �� ����������� ��� ����������
        if (v.bound > max_profit) Q.push(v);           // ��� ������� ��������� ��������� ����� ��������� �� ����������� ���� ���� ��� ������
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
        last_sol_time = duration.count()/1000.0;
        if (last_sol_time > time_out ) {
            std::cout << "out of time: time>" << time_out << "\n";

            return selectedItems;
        }

    }
    std::cout << "max_weight=" << max_weight << "\n";
    std::cout << "max_profit=" << max_profit << "\n";
    std::cout << "time=" << last_sol_time << "\n";
    return selectedItems;
}

std::vector <item>  branch_and_bound_solver(knapsack_problem& ks) {

    int n = ks.items.size();
    item* arr = new item[n];
    for (int i = 0; i < n; i++) arr[i] = ks.items.at(i);
    std::cout << "Branch and Bound Solver\n";
    std::vector <int> solutionIDs = branch_and_bound(ks.capacity, arr, n);
    std::vector <item> solution;
    for (int i = 0; i < solutionIDs.size(); i++) {       // ������������ ��� ����� �� �� ����������� ��� ����������� ��� ����
        int itemIndex = solutionIDs[i] - 1;
        solution.push_back(ks.items[itemIndex]);
    }
    delete[] arr;
    return solution;
}
/*4 Dynamic Programming Solver */
int max(int x, int y) {
    return (x > y) ? x : y;
}
std::vector <item> dynamic_programming_solver(knapsack_problem& ks) {
    std::cout << "Dynamic Programming Solver\n";
    auto begin = std::chrono::high_resolution_clock::now();
    bool isOutOfTime = false;
    int W = ks.capacity;                      // ������������ ��� ������������ ��� �������� ���� W
    int n = ks.items.size();                   // ������������ �o� ������ ��� ������������ ��� n
    std::vector<int> w(n, 0);   // create a vector to hold "size" int's
    std::vector<int> v(n, 0);   // create a vector to hold "size" int's
    int m = 0;
    for (auto& tempItem : ks.items) {
        w[m] = tempItem.weight;  // �������� ��� ������ w �������� n  ��� �� ������������ �� ����� ���� ������������
        v[m] = tempItem.profit; // �������� ��� ������ v �������� n  ��� �� ������������ ��� ���� ���� �����������
        m++;
    }
    int i, wt;
    int n_ = n + 1;
    int W_ = W + 1;
    std::vector<std::vector <int> > K(n_, std::vector<int>(W_, 0));

    for (i = 0; i <= n; i++) {                 // �������� ���� ��� ������� ��� ������ � ��� ���������� ��� �� ����������� (+ 1 �������� ����� ��� �����)

        for (wt = 0; wt <= W; wt++) {           // �������� ���� ��� ������
            if (i == 0 || wt == 0) K[i][wt] = 0; // ������� 0 ��� ����� ������ ��� ����� ��� ������
            else if (w[(i - 1)] <= wt)             // ��� �� ����� ��� ������������ ������������ ����� ��������� � ��� �� ��� ������ ��� ������
                K[i][wt] = max(v[(i - 1)] + K[i - 1][wt - w[(i - 1)]], K[i - 1][wt]); // ������������ ��� ��� ����� ��� ���� ��� ���������  ��� ������
            else
                K[i][wt] = K[i - 1][wt];              // ����������� �� ������� ��� �� �� �������� ��� ������������ ������� ��� ���� ������
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
        last_sol_time = duration.count()/1000.0;
        if (last_sol_time > time_out) {
            std::cout << "out of time: time>" << time_out << "\n";
           isOutOfTime = true;
            break;
        }
    }


    std::vector <item> solution;                    // ������������ �� vector ��� �� �������� �� ���������� ��� ���� �����������
    if (!isOutOfTime) {
        while (n != 0) {                            // �� n �������� ��� ������ ��� ������������ ��� ���� ������ ������� ���� ��������� ������
                                                    // �������� ��� �� �������� ��� ������ (����������) ��� ��� ��������� ����� ��� ������ ���� ��� ���� ���
            if (K[n][W] != K[n - 1][W]) {           // ��������� �� �� ������ �������� ��� ������ ����� ����������� ��� �� �����������
                                                    // �� ��������� ��� �����, �� ����������� �������� ����� ��� �����
                item sol = ks.items.at((n - 1));    // ������������ ��� struct sol ��� �� ������������ �� ����������� ��� ���� ��������� ��� ����
                solution.push_back(sol);            // ����������� �� ����������� ��� ����� �� �� ���������� �����������
                W = W - w[(n - 1)];                 // ����������� ��� ������ ��� ��� ����������� �����. ������ ���� ���� ��� ��������� ��� ����� ��� �����
                                                    // ��������� ����� ����������� ��� ����
            }
            n--;                                    // ��� �� ��� ������ ����� ��� ����� ��������� ������ ����������� ��� ������ ���� ����� �� ��������� ���
                                                    // ��� ��� ������� ������ ��� ������ (� ����� ����� �����������)
        }

   }
        auto finish = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - begin);
        last_sol_time = duration.count()/1000.0;
   for (int i = 0; i < n + 1; ++i) {

        K[i].clear();
    }
    K.clear();
    v.clear();
    w.clear();
    return solution;                            // ������������ �� vector solution �� ����� �������� �� ����� �� �� ���������� ��� ���� �����������
}
/* print_solution
   � print_solution ������� �� ��������� ��� ���� ���� ����������� knapsack_problem
   ��� ��������� �� �������� ���
 */
void print_solution(std::vector<item> sol) {
   // std::string output;
    int total_weight = 0;
    int total_profit = 0;

    for (int i = 0; i < sol.size(); i++) {
        std::cout << sol[i].id << " " << sol[i].profit << " " << sol[i].weight << "\n";
        total_weight += sol[i].weight;
        total_profit += sol[i].profit;
    }
    std::cout << "Total weight:" << total_weight << "\n";
    std::cout << "Total profit:" << total_profit << "\n";


}
/* export_solution

   � export_solution  ���������� �� ���� ���� knapsach ����������� �� ��� ������
 */
void export_solution(knapsack_problem& ks, std::vector<item> sol, std::string& fn)
{   std::fstream fout;
    fout.open(fn, std::ios::out);

    if (!fout.good()) {
        std::cerr << "Error accesing file" << fn << std::endl;
        exit(-1);
    }

    std::string output;
     int total_weight = 0;
    int total_profit = 0;
    for (int i = 0;i < sol.size();i++)
    {
        fout << sol[i].id<<" "<<sol[i].profit<<" "<< sol[i].weight<<"\n";
        total_weight += sol[i].weight;
        total_profit += sol[i].profit;
    }
    fout << " ";
    fout << total_profit;
    fout << " ";
    fout << total_weight;
    fout << "\n";

    fout.close();
}

void calc_p_w(std::vector<item>& solution, std::fstream& fout) {
    int w = 0;
    int v = 0;
    for (int i = 0; i < solution.size(); i++) {
        item tmpItem = solution.at(i);
        w += tmpItem.weight;
        v += tmpItem.profit;
    }
    std::string s = ";" + std::to_string(w) + ";" + std::to_string(v);
    fout << s;
}
/*
   � export_csv ������� ��� ������ �� ����� ���� �������� ����������� ��� ������ �� ���������� �� ������ CSV
 */
void export_csv(int choice) {
    int n1[] = { 10,50,100,500 };
    int r1[] = { 50,100,500,1000 };
    int type1[] = { 1,2,3,4 };
    std::string fname_string;
    int file_counter = 0;
    std::string csv_path = "_results.csv";
    std::remove(csv_path.c_str());
    std::fstream fout;
    std::cout << "----- csv_path=" << csv_path << "\n";
    fout.open(csv_path, std::ios::out);
    if (!fout.good()) {     // ��������� �� ��� ���� ������������
        std::cerr << "--- Error opening file " << csv_path << std::endl;   // ����������� ������ ������ �� ���� ��� ���������
        system("pause");
        exit(-1);
    }
     std::string header = "INSTANCE";
     header.append(";WEIGHT_GR;VALUE_GR;EXECUTION_TIME_GR");
     header.append(";WEIGHT_BF;VALUE_BF;EXECUTION_TIME_BF");
     header.append(";WEIGHT_BB;VALUE_BB;EXECUTION_TIME_BB");
     header.append(";WEIGHT_DP;VALUE_DP;EXECUTION_TIME_DP");
     header.append(";WEIGHT_OT;VALUE_OT;EXECUTION_TIME_OT");
     header.append(";WEIGHT_IP;VALUE_IP;EXECUTION_TIME_IP");
     header.append("\n");
     fout << header;

    for (int k = 0; k < 4; k++) {
        for (int l = 0; l < 4; l++) {
            for (int m = 0; m < 4;m++) {
                for (int o = 1; o <= 5; o++) {
                    int n = n1[k];
                    int r = r1[l];
                    int type = type1[m];
                    int i = o;
                    int S = 5;
                    std::vector<item> result;

                    fname_string = "problem_" + std::to_string(n) + "_" + std::to_string(r) + "_" + std::to_string(type) + "_" + std::to_string(i) + "_5.txt";
                    file_counter++;
                    std::cout << "=======" << file_counter << "========\n";
                    std::cout << "filename= " << fname_string << "\n";
                    fout << ";" << fname_string;
                    knapsack_problem ks = read_data(fname_string);
                    std::cout << "total capacity=" << ks.capacity << "\n";
                    std::cout << "size=" << ks.items.size() << "\n";

                    // greedy approach solver
                    if (choice == 0) {
                        result.clear();
                        result = greedy_approach_solver(ks);
                        calc_p_w(result, fout);
                        fout << ";" << last_sol_time;
                    }

                    // Brute force solver
                    if (choice == 0) {
                        result.clear();
                        result = brute_force_solver(ks);
                        calc_p_w(result, fout);
                        fout << ";" << last_sol_time;
                    }

                    // Branch and bound solver
                    if (choice == 0)  {
                        result.clear();
                        result = branch_and_bound_solver(ks);
                        calc_p_w(result, fout);
                        fout << ";" << last_sol_time;

                    }

                    // Dynamic programming solver
                    if (choice == 0)  {
                        result.clear();
                        result = dynamic_programming_solver(ks);
                        calc_p_w(result, fout);
                        fout << ";" << last_sol_time;
                    }

                    // Dynamic OR-Tools solver
                    if (choice == 0) {
                        fout.close();
                        std::string fname = "dynamic_prog_ortools.exe " + fname_string + " " + csv_path;
                        std::cout << "Filename: " << fname << "\n";
                        int code = system(fname.c_str());
                        fout.open(csv_path, std::ios::app);
                    }


                    //Integer OR-Tools solver
                    if (choice == 0) {
                        fout.close();
                        std::string fname = "integer_prog_ortools.exe " + fname_string + " " + csv_path;
                        std::cout << "Filename: " << fname << "\n";
                        int code = system(fname.c_str());
                        fout.open(csv_path, std::ios::app);
                    }

                    fout << "\n";
                }
            }
        }
    }

    fout.close();
    std::cout << "Files: " << file_counter << "\n";
}

    int main() {
        int choice = -1;
        do {
            system("CLS");
            std::cout << "|=============Copyright (c) 2019 - All rights reserved===============|\n";
            std::cout << "|======================Aristotelis Zacharis==========================|\n";
            std::cout << "|====================================================================|\n";
            std::cout << "|========================0-1 Knapsack Problem========================|\n";
            std::cout << "|====================================================================|\n";
            std::cout << "Please choice the algorithm to solve 0-1 Knapsack Problem: \n" << std::endl;
            std::cout << "1. Greedy approach solver" << std::endl;
            std::cout << "2. Brute force solver " << std::endl;
            std::cout << "3. Branch and bound solver " << std::endl;
            std::cout << "4. Dynamic programming solver" << std::endl;
            std::cout << "5. Dynamic programming by ortools" << std::endl;
            std::cout << "6. Integer Programming by ortools " << std::endl;
            std::cout << "7. Export CSV with all algorithms" << std::endl;
            std::cout << "0. Exit\n" << std::endl;
            std::cout << "Please enter your choice" << std::endl;
            std::cin >> choice;
            if ((choice < 0) || (choice > 7)) {
                std::cout << "Wrong choice!\n";
                system("pause");
                exit(0);
            }
            if (choice == 0) {
                exit(0);
                system("pause");
            }
            if (choice == 7) {
                export_csv(0);
                system("pause");
            }

            else {
                int n;
                int r;
                int type;
                int i;
                int s;
                std::cout << "Enter the 5 parameters for 0-1 knapsack problem to print and export the solution: \n";
                std::cout << "Enter the parameter n (10,50,100,500) n: ";
                std::cin >> n;
                std::cout << "Enter the parameter r (50,100,500,1000) r: ";
                std::cin >> r;
                std::cout << "Enter the parameter type(1,2,3,4) type: ";
                std::cin >> type;
                std::cout << "Enter the parameter i (1,2,3,4,5) i: ";
                std::cin >> i;
                std::cout << "Enter the number 5 in the parameter s: ";
                std::cin >> s;
                std::string  fn = "problem_" + std::to_string(n) + "_" + std::to_string(r) + "_" + std::to_string(type) + "_" + std::to_string(i) + "_" + std::to_string(s) + ".txt";
                std::string solution_fn = "solution_" + std::to_string(n) + "_" + std::to_string(r) + "_" + std::to_string(type) + "_" + std::to_string(i) + "_" + std::to_string(s) + ".txt";
                std::cout << "Filename=" << fn << "\n";
                knapsack_problem ks = read_data(fn);
                std::cout << "total capacity=" << ks.capacity << "\n";
                std::string fname;

                std::vector<item> result;
                switch (choice) {
                case 1: result = greedy_approach_solver(ks);
                    print_solution(result);
                    export_solution(ks, result, solution_fn);
                    break;
                case 2: result = brute_force_solver(ks);
                    print_solution(result);
                    export_solution(ks, result, solution_fn);
                    break;
                case 3: result = branch_and_bound_solver(ks);
                    print_solution(result);
                    export_solution(ks, result, solution_fn);
                    break;
                case 4: result = dynamic_programming_solver(ks);
                    print_solution(result);
                    export_solution(ks, result, solution_fn);
                    break;
                case 5: fname = "dynamic_prog_ortools.exe " + fn + " " + solution_fn;
                    std::cout << "Filename: " << fname << "\n";
                    system(fname.c_str());
                    break;
                case 6: fname = "integer_prog_ortools.exe " + fn + " " + solution_fn;
                    std::cout << "Filename: " << fname << "\n";
                    system(fname.c_str());
                    break;
                default: break;
                }
                system("pause");
            }
        }  while (choice != 0);
        exit(0);
    }
