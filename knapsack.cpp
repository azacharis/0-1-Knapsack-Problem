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

struct item
{
    int id;         
    int profit;     
    int weight;     
    int bound;      
    int level;      
    std::vector<int> itemList; 
};
int time_out = 10;
// Ïñßæïõìå Ýíá struct ôï ïðïßï èá áðïèçêåýåé óå ìéá ëßóôá ìå ôá åðéìÝñïõò óôïé÷åßá
// ôùí áíôéêåéìÝíùí êáé ôç óõíïëéêÞ ÷ùñéêüôçôá ôïõ óáêéäßïõ.
// Ãéá ôçí ðåñßðôùóç ðïõ ï õðïëïãéóìüò ìðïñåß íá äéáñêÝóåé ðïëý ïñßæïõìå êáé Ýíá ÷ñüíï äéáêïðÞò ôçò äéáäéêáóßáò
struct knapsack_problem
{
    std::vector<item> items;     // Ëßóôá ìå ôá óõíïëéêÜ áíôéêåßìåíá
    int capacity;           // ×ùñçôéêüôçôá óáêéäßïõ
    int time_limit = 10;      // ×ñüíïò äéáêïðÞò ôïõ õðïëïãéóìïý áí îåðåñáóôåß óå  seconds
};
int last_sol_time = 0;
/*read_data

   Ç óõíÜñôçóç read_data äÝ÷åôáé ùò ðáñÜìåôñï ôï üíïìá åíüò áñ÷åßïõ ìå äåäïìÝíá knapsack êáé
   åðéóôñÝöåé ôï ðåñéå÷üìåíï óå ìïñöÞ (struct) knapsack_problem
 */
knapsack_problem read_data(std::string& fn) {
    std::ifstream fin(fn);     // Ðñïóðáèïýìå íá áíïßîïõìå ôï áñ÷åßï
    if (!fin.good()) {     // ÅëÝã÷ïõìå áí äåí Ý÷åé áíïé÷èåß
        std::cerr << "Error opening file " << fn << std::endl;   // åêôõðþíïõìå ìÞíõìá ëÜèïõò óå áõôÞ ôçí ðåñßðôùóç
        system("pause");
        exit(-1);         // ÐÜìå ðëáôåßá ãéá êáöÝ
    }

    knapsack_problem ks;  // Äçìéïõñãïýìå ìéá ìåôáâëçôÞ ôýðïõ knapsack_problem
                          // óå áõôÞ èá áðïèçkåõôïýí ôá óôïé÷åßá ôïõ áñ÷åßïõ knapsack
    int items_number;     // Ïñßæïõìå ìéá ìåôáâëçôÞ items_number ãéá íá áðïèçêåýïõìå ôïí
                          // áñéèìü ôùí áíôéêåéìÝíùí ðïõ âñßóêïíôáé óôï áñ÷åßï
    fin >> items_number;  // ÄéáâÜæïõìå áðü ôçí ðñþôç ãñáììÞ ôïí áñéèìü ôùí áíôéêåéìÝíùí êáé ôïí êñáôÜìå óôçí items_number
    for (int i = 0; i < items_number; i++)  // Ãéá üóá åßíáé ôá áíôéêåßìåíá ôïõ áñ÷åßïõ èá ôñÝîïõìå ôïí ðáñáêÜôù êþäéêá
                          // ãéá íá äéáâÜóïõìå ôá óôïé÷åßá êÜèå áíôéêåéìÝíïõ
    {
        item an_item;     // Ç ìåôáâëçôÞ áõôÞ êñáôÜåé êÜèå öïñÜ ôá óôïé÷åßá êÜèå áíôéêåéìÝíïõ áðü ôï áñ÷åßï
        fin >> an_item.id;      // ÄéáâÜæïõìå ôï id (óåéñÜ ôïõ áíôéêåéìÝíïõ óôï áñ÷åßï)
        fin >> an_item.profit;  // ÄéáâÜæïõìå ôçí áîßá êáé ôçí áðïèçêåýïõìå óôçí an_item.profit
        fin >> an_item.weight;  // ÄéáâÜæïõìå ôï âÜñïò êáé ôï áðïèçêåýïõìå óôï an_item.weight
        ks.items.push_back(an_item);  // ÅéóÜãïõìå ôï an_item óôï ôÝëïò ôçò ëßóôáò ôïõ ks (knapsack_problem)
    }
    fin >> ks.capacity;         // ÔÝëïò äéáâÜæïõìå ôçí ÷ùñçôéêüôçôá ôïõ óáêéäßïõ êáé ôçí áðïèçêåýïõìå óôï ks.capacity
    return ks;  // åðéóôñÝöïõìå ìéá äïìÞ ôýðïõ knapsack_problem ç ïðïßá ðåñéÝ÷åé üëá ôá óôoé÷åßá ôïõ óõãêåêñéìÝíïõ ðñïâëÞìáôïò
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
    std::vector<item> itemsHolder(ks.items);    // äçìéïõñãïýìå Ýíá áíôéãñáöï ôïõ ðßíáêá ôùí áíôéêåéìÝíùí þóôå íá ãßíåé ôáîéíüìéóç ìüíï óôï áíôéãñáöï êáé íá ìçí áëëáîåé ç óåéñÜ óôïí áñ÷éêü ðßíáêá
    sort(itemsHolder.begin(), itemsHolder.end(), cmp);
    int current_weight = 0;              // ÔñÝ÷ïí óõíïëéêü âÜñïò êáôá ôïí õðïëïãéóìü
    double finalprofit = 0.0;        // Ðïóïóôü åðéôõ÷ßáò: óõíïëéêü âÜñïò áíôéêåéìÝíùí ôïõ óáêéäßïõ / ÷ùñçôéêüôçôá
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

/*2  Ârute Force Solver get_profit */
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

    while (!Q.empty()) {        // Ç ïõñÜ ðåñéÝ÷åé ôá áíôéêåßìåíá ðïõ ðñ´ðåé íá åëåãèïýí. Åöüóïí äåí åßíáé Üäåéá óõíå÷ßæïõìå
        u = Q.front();          // ÊñáôÜìå óôç u ôï ðñþôï áíôéêåßìåíï áðü ôçí ïõñÜ
        Q.pop();                // êáé ìåôÜ ôï áöáéñïýìå áöïý èá åëåãèåß ðáñáêÜôù
        if (u.level == -1) v.level = 0;    // Ôçí ðñþôç öïñÜ ðïõ èá åêôåëåóèåß ôï loop ç u.level åßíáé -1 êáé ôçí êÜíïõìå 0
        if (u.level == n - 1) continue;    // åÜí öôÜóïõìå óôï ôåëåõôáßï áíôéêåßìåíï óôáìáôÜ ìå êáé ðÜìå íá åîåôÜóïõìå ôï åðüìåíï ôçò ïõñÜò
        v.level = u.level + 1;  // Ï Ýëåã÷ïò èá ãßíåé óôï åðüìåíï level (áíôéêåßìåíï)
        v.weight = u.weight + arr[v.level].weight;    //áðïèçêåýïõìå óôç v ôï åðüìåíï áíôéêåßìåíï êáé ðñïóèÝôïõìå ôï ìÝ÷ñé ôþñá âÜñïò êáé áîßá
        v.profit = u.profit + arr[v.level].profit;
        v.id = arr[v.level].id;
        v.itemList.clear();                           // ßóùò ôæÜìðá íá ôï Ýâáëá áõôü. Íá ôï îáíáäù óôï ôÝëïò
        v.itemList = u.itemList;
        v.itemList.push_back(v.id);
        if (v.weight <= W && v.profit > max_profit) {  // åÜí ôï âÜñïò äåí Ý÷åé îåðåñÜóåé ôï ìÝãéóôï âÜñïò êáé ç áîßá ôïõ äåí åßíáé ìéêñüôåñç áðü áõôÞ ðïõ ìÝ÷ñé ôþñá Ý÷ïõìå
            max_profit = v.profit;                     // åðéëÝãïõìå ôï áíôéêåßìåíï êñáôþíôáò ôï ùò êáëýôåñç ìÝ÷ñé ôþñá åðéëïãÞ ãéá ôï âÜñïò êáé ôçí áîßá
            max_weight = v.weight;
            selectedItems = v.itemList;               // êñáôÜìå êáé ôç ëßóôá ìå ôá åðéëåãìÝíá áíôéêåßìåíá ôïõ óõíäéáóìïý ùò ç êáëýôåñç ìÝ÷ñé ôþñá
        }
        v.bound = bound(v, n, W, arr);                // õðïëïãßæïõìå ôçí ìÝãéóôç áîßá ãéá üóá áíôéêåßìåíá áêïëïõèïýí
        if (v.bound > max_profit) {                    // áí ç ìÝãéóôç áîßá åßíáé êáëýôåñç áðü áõôÞ ðïõ Ý÷ïõìå
            Q.push(v);                                // èá ðñÝðåé íá åëåãèåß ï êëÜäïò ôïõ v
        }
        v.weight = u.weight;                          // ôþñá èá åéóÜãïõìå îáíÜ ôï áíôéêåßìåíï óôçí ïõñÜ ãéá Ýëåã÷ï áëëÜ áõôÞ ôçí öïñÜ äåí èá óõìðåñéëÜâïõìå ôçí äéêÞ ôïõ áîßá êáé âÜñïò
        v.profit = u.profit;                          // ïé ëýóåéò ðïõ èá äçìéïõñãçèïýí äåí èá óõìðåñéëáìâÜíïõí ôï ôñÝ÷ïí áíôéêåßìåíï
        v.id = u.id;
        v.itemList = u.itemList;
        v.bound = bound(v, n, W, arr);                // õðïëïãßæïõìå îáíÜ ôçí ìÝãéóôç äõíáôÞ áîßá ãéá ôá áíôéêåßìåíá ðïõ áêïëïõèïýí
        if (v.bound > max_profit) Q.push(v);           // åÜí õðÜñ÷åé ðåñßðôùóç êáëýôåñçò ëýóçò åéóÜãïõìå ôï áíôéêåßìåíï óôçí ïõñÜ ãéá Ýëåã÷ï
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
    for (int i = 0; i < solutionIDs.size(); i++) {       // Äçìéïõñãïýìå ìéá ëßóôá ìå ôá áíôéêåßìåíá ðïõ åðéëÝ÷èçêáí óôç ëýóç
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
    int W = ks.capacity;                      // áðïèçêåýïõìå ôçí ÷ùñçôéêüôçôá ôïõ óáêéäßïõ óôçí W
    int n = ks.items.size();                   // áðïèçêåýïõìå ôoí áñéèìü ôùí áíôéêåéìÝíùí óôï n
    std::vector<int> w(n, 0);   // create a vector to hold "size" int's
    std::vector<int> v(n, 0);   // create a vector to hold "size" int's
    int m = 0;
    for (auto& tempItem : ks.items) {
        w[m] = tempItem.weight;  // ïñßæïõìå Ýíá ðßíáêá w ìåãÝèïõò n  ãéá íá áðïèêåýóïõìå ôï âÜñïò êÜèå áíôéêåéìÝíïõ
        v[m] = tempItem.profit; // ïñßæïõìå Ýíá ðßíáêá v ìåãÝèïõò n  ãéá íá áðïèêåýóïõìå ôçí áîßá êÜèå áíôéêåéìÝíï
        m++;
    }
    int i, wt;
    int n_ = n + 1;
    int W_ = W + 1;
    std::vector<std::vector <int> > K(n_, std::vector<int>(W_, 0));

    for (i = 0; i <= n; i++) {                 // ôñÝ÷ïõìå üëåò ôéò ãñáììÝò ôïõ ðßíáêá Ê êáé ôáõôü÷ñïíá üëá ôá áíôéêåßìåíá (+ 1 óôïé÷åßï áêüìç óôï ôÝëïò)

        for (wt = 0; wt <= W; wt++) {           // ôñÝ÷ïõìå üëåò ôéò óôÞëåò
            if (i == 0 || wt == 0) K[i][wt] = 0; // êÜíïõìå 0 ôçí ðñþôç ãñáììÞ êáé óôÞëç ôïõ ðßíáêá
            else if (w[(i - 1)] <= wt)             // åÜí ôï âÜñïò ôïõ ðñïçãïýìåíïõ áíôéêåéìÝíïõ åßíáé ìéêñüôåñï Þ ßóï ìå ôïí áñéèìü ôçò óôÞëçò
                K[i][wt] = max(v[(i - 1)] + K[i - 1][wt - w[(i - 1)]], K[i - 1][wt]); // õðïëïãßæïõìå áðü ôçí ó÷Ýóç ôçí ôéìÞ ôïõ óôïé÷åßïõ  ôïõ ðßíáêá
            else
                K[i][wt] = K[i - 1][wt];              // äéáöïñåôéêÜ ôï êÜíïõìå ßóï ìå ôï óôïé÷åßï ôçò ðñïçãïýìåíçò ãñáììÞò ôçò ßäéá óôÞëçò
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


    std::vector <item> solution;                    // äçìéïõñãïýìå ôï vector ðïõ èá ðåñéÝ÷åé ôá åðéëåãìÝíá óôç ëýóç áíôéêåßìåíá
    if (!isOutOfTime) {
        while (n != 0) {                            // ôï n ðåñéÝ÷åé ôçí áñéèìü ôùí áíôéêåéìÝíùí êáé óôïí ðßíáêá äåß÷íåé óôçí ôåëåõôáßá ãñáììÞ
                                                    // ôñÝ÷ïõìå üëá ôá óôïé÷åßá ôïõ ðßíáêá (áíôßóôñïöá) áðü ôçí ôåëåõôáßá óôÞëç êáé ãñáììÞ ðñïò ôçí áñ÷Þ ôïõ
            if (K[n][W] != K[n - 1][W]) {           // åëÝã÷ïõìå áí ôï ôñÝ÷ïí óôïé÷åßï ôïõ ðßíáêá åßíáé äéáöïñåôéêü áðü ôï ðñïçãïýìåíï
                                                    // óå ðåñßðôùóç ðïõ åßíáé, ôï áíôéêåßìåíï áðïôåëåß ìÝñïò ôçò ëýóçò
                item sol = ks.items.at((n - 1));    // äçìéïõñãïýìå Ýíá struct sol ãéá íá áðïèçêåýïõìå ôï áíôéêåßìåíï ðïõ Ý÷åé åðéëå÷èåß óôç ëýóç
                solution.push_back(sol);            // ðñïóèÝôïõìå ôï áíôéêåßìåíï óôç ëßóôá ìå ôá åðéëåãìÝíá áíôéêåßíáìå
                W = W - w[(n - 1)];                 // óõíå÷ßæïõìå ôïí Ýëåã÷ï áðü ôçí ðñïçãïýìåíç óôÞëç. ÄçëáäÞ êÜèå öïñÜ ðïõ âñßóêïõìå Ýíá ìÝñïò ôçò ëýóçò
                                                    // áëëÜæïõìå óôÞëç ðçãáßíïíôáò ìßá ðßóù
            }
            n--;                                    // åíþ áí äåí âñïýìå ìÝñïò ôçò ëýóçò áëëÜæïõìå ãñáììÞ ðçãáßíïíôáò ìßá ãñáììÞ ðßóù ìÝ÷ñé íá åëÝãîïõìå êáé
                                                    // êáé ôçí äåýôåñç ãñáììÞ ôïõ ðßíáêá (ç ðñþôç åßíáé ìçäåíéóìÝíç)
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
    return solution;                            // åðéóôñÝöïõìå ôï vector solution ôï ïðïßï ðåñéÝ÷åé ôç ëéóôá ìå ôá åðéëåãìÝíá óôç ëýóç áíôéêåßìåíá
}
/* print_solution
   Ç print_solution äÝ÷åôáé ùò ðáñÜìåôñï ôçí ëýóç åíüò ðñïâëÞìáôïò knapsack_problem
   êáé åêôõðþíåé ôá óôïé÷åßá ôïõ
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

   Ç export_solution  áðïèçêåýåé ôç ëýóç åíüò knapsach ðñïâëÞìáôïò óå Ýíá áñ÷åßï
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
   Ç export_csv âñßóêåé ôéò ëýóåéò ìå üëïõò ôïõò äõíáôïýò áëãüñéèìïõò êáé åîÜãåé ôï áðïôÝëåóìá óå áñ÷åßï CSV
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
    if (!fout.good()) {     // ÅëÝã÷ïõìå áí äåí Ý÷åé äçìéïõñãçèåß
        std::cerr << "--- Error opening file " << csv_path << std::endl;   // åêôõðþíïõìå ìÞíõìá ëÜèïõò óå áõôÞ ôçí ðåñßðôùóç
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
