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
// Η struct item περιέχει τα στοιχεία κάθε αντικειμένου
struct item
{
    int id;         // αριθμός αντικειμένου με τη σειρά που βρίσκεται στο αρχείο
    int profit;     // αξία
    int weight;     // βάρος
    int bound;      //φραγη
    int level;      //επίπεδο δεντρου
    std::vector<int> itemList; //λιστα για τις λυσεις του branch and bound
};
int time_out = 10;
// Ορίζουμε ένα struct το οποίο θα αποθηκεύει σε μια λίστα με τα επιμέρους στοιχεία
// των αντικειμένων και τη συνολική χωρικότητα του σακιδίου.
// Για την περίπτωση που ο υπολογισμός μπορεί να διαρκέσει πολύ ορίζουμε και ένα χρόνο διακοπής της διαδικασίας
struct knapsack_problem
{
    std::vector<item> items;     // Λίστα με τα συνολικά αντικείμενα
    int capacity;           // Χωρητικότητα σακιδίου
    int time_limit = 10;      // Χρόνος διακοπής του υπολογισμού αν ξεπεραστεί σε  seconds
};
int last_sol_time = 0;
/*read_data

   Η συνάρτηση read_data δέχεται ως παράμετρο το όνομα ενός αρχείου με δεδομένα knapsack και
   επιστρέφει το περιεχόμενο σε μορφή (struct) knapsack_problem
 */
knapsack_problem read_data(std::string& fn) {
    std::ifstream fin(fn);     // Προσπαθούμε να ανοίξουμε το αρχείο
    if (!fin.good()) {     // Ελέγχουμε αν δεν έχει ανοιχθεί
        std::cerr << "Error opening file " << fn << std::endl;   // εκτυπώνουμε μήνυμα λάθους σε αυτή την περίπτωση
        system("pause");
        exit(-1);         // Πάμε πλατεία για καφέ
    }

    knapsack_problem ks;  // Δημιουργούμε μια μεταβλητή τύπου knapsack_problem
                          // σε αυτή θα αποθηkευτούν τα στοιχεία του αρχείου knapsack
    int items_number;     // Ορίζουμε μια μεταβλητή items_number για να αποθηκεύουμε τον
                          // αριθμό των αντικειμένων που βρίσκονται στο αρχείο
    fin >> items_number;  // Διαβάζουμε από την πρώτη γραμμή τον αριθμό των αντικειμένων και τον κρατάμε στην items_number
    for (int i = 0; i < items_number; i++)  // Για όσα είναι τα αντικείμενα του αρχείου θα τρέξουμε τον παρακάτω κώδικα
                          // για να διαβάσουμε τα στοιχεία κάθε αντικειμένου
    {
        item an_item;     // Η μεταβλητή αυτή κρατάει κάθε φορά τα στοιχεία κάθε αντικειμένου από το αρχείο
        fin >> an_item.id;      // Διαβάζουμε το id (σειρά του αντικειμένου στο αρχείο)
        fin >> an_item.profit;  // Διαβάζουμε την αξία και την αποθηκεύουμε στην an_item.profit
        fin >> an_item.weight;  // Διαβάζουμε το βάρος και το αποθηκεύουμε στο an_item.weight
        ks.items.push_back(an_item);  // Εισάγουμε το an_item στο τέλος της λίστας του ks (knapsack_problem)
    }
    fin >> ks.capacity;         // Τέλος διαβάζουμε την χωρητικότητα του σακιδίου και την αποθηκεύουμε στο ks.capacity
    return ks;  // επιστρέφουμε μια δομή τύπου knapsack_problem η οποία περιέχει όλα τα στoιχεία του συγκεκριμένου προβλήματος
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
    std::vector<item> itemsHolder(ks.items);    // δημιουργούμε ένα αντιγραφο του πίνακα των αντικειμένων ώστε να γίνει ταξινόμιση μόνο στο αντιγραφο και να μην αλλαξει η σειρά στον αρχικό πίνακα
    sort(itemsHolder.begin(), itemsHolder.end(), cmp);
    int current_weight = 0;              // Τρέχον συνολικό βάρος κατα τον υπολογισμό
    double finalprofit = 0.0;        // Ποσοστό επιτυχίας: συνολικό βάρος αντικειμένων του σακιδίου / χωρητικότητα
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

/*2  Βrute Force Solver get_profit */
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

    while (!Q.empty()) {        // Η ουρά περιέχει τα αντικείμενα που πρ΄πει να ελεγθούν. Εφόσον δεν είναι άδεια συνεχίζουμε
        u = Q.front();          // Κρατάμε στη u το πρώτο αντικείμενο από την ουρά
        Q.pop();                // και μετά το αφαιρούμε αφού θα ελεγθεί παρακάτω
        if (u.level == -1) v.level = 0;    // Την πρώτη φορά που θα εκτελεσθεί το loop η u.level είναι -1 και την κάνουμε 0
        if (u.level == n - 1) continue;    // εάν φτάσουμε στο τελευταίο αντικείμενο σταματά με και πάμε να εξετάσουμε το επόμενο της ουράς
        v.level = u.level + 1;  // Ο έλεγχος θα γίνει στο επόμενο level (αντικείμενο)
        v.weight = u.weight + arr[v.level].weight;    //αποθηκεύουμε στη v το επόμενο αντικείμενο και προσθέτουμε το μέχρι τώρα βάρος και αξία
        v.profit = u.profit + arr[v.level].profit;
        v.id = arr[v.level].id;
        v.itemList.clear();                           // ίσως τζάμπα να το έβαλα αυτό. Να το ξαναδω στο τέλος
        v.itemList = u.itemList;
        v.itemList.push_back(v.id);
        if (v.weight <= W && v.profit > max_profit) {  // εάν το βάρος δεν έχει ξεπεράσει το μέγιστο βάρος και η αξία του δεν είναι μικρότερη από αυτή που μέχρι τώρα έχουμε
            max_profit = v.profit;                     // επιλέγουμε το αντικείμενο κρατώντας το ως καλύτερη μέχρι τώρα επιλογή για το βάρος και την αξία
            max_weight = v.weight;
            selectedItems = v.itemList;               // κρατάμε και τη λίστα με τα επιλεγμένα αντικείμενα του συνδιασμού ως η καλύτερη μέχρι τώρα
        }
        v.bound = bound(v, n, W, arr);                // υπολογίζουμε την μέγιστη αξία για όσα αντικείμενα ακολουθούν
        if (v.bound > max_profit) {                    // αν η μέγιστη αξία είναι καλύτερη από αυτή που έχουμε
            Q.push(v);                                // θα πρέπει να ελεγθεί ο κλάδος του v
        }
        v.weight = u.weight;                          // τώρα θα εισάγουμε ξανά το αντικείμενο στην ουρά για έλεγχο αλλά αυτή την φορά δεν θα συμπεριλάβουμε την δική του αξία και βάρος
        v.profit = u.profit;                          // οι λύσεις που θα δημιουργηθούν δεν θα συμπεριλαμβάνουν το τρέχον αντικείμενο
        v.id = u.id;
        v.itemList = u.itemList;
        v.bound = bound(v, n, W, arr);                // υπολογίζουμε ξανά την μέγιστη δυνατή αξία για τα αντικείμενα που ακολουθούν
        if (v.bound > max_profit) Q.push(v);           // εάν υπάρχει περίπτωση καλύτερης λύσης εισάγουμε το αντικείμενο στην ουρά για έλεγχο
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
    for (int i = 0; i < solutionIDs.size(); i++) {       // Δημιουργούμε μια λίστα με τα αντικείμενα που επιλέχθηκαν στη λύση
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
    int W = ks.capacity;                      // αποθηκεύουμε την χωρητικότητα του σακιδίου στην W
    int n = ks.items.size();                   // αποθηκεύουμε τoν αριθμό των αντικειμένων στο n
    std::vector<int> w(n, 0);   // create a vector to hold "size" int's
    std::vector<int> v(n, 0);   // create a vector to hold "size" int's
    int m = 0;
    for (auto& tempItem : ks.items) {
        w[m] = tempItem.weight;  // ορίζουμε ένα πίνακα w μεγέθους n  για να αποθκεύσουμε το βάρος κάθε αντικειμένου
        v[m] = tempItem.profit; // ορίζουμε ένα πίνακα v μεγέθους n  για να αποθκεύσουμε την αξία κάθε αντικειμένο
        m++;
    }
    int i, wt;
    int n_ = n + 1;
    int W_ = W + 1;
    std::vector<std::vector <int> > K(n_, std::vector<int>(W_, 0));

    for (i = 0; i <= n; i++) {                 // τρέχουμε όλες τις γραμμές του πίνακα Κ και ταυτόχρονα όλα τα αντικείμενα (+ 1 στοιχείο ακόμη στο τέλος)

        for (wt = 0; wt <= W; wt++) {           // τρέχουμε όλες τις στήλες
            if (i == 0 || wt == 0) K[i][wt] = 0; // κάνουμε 0 την πρώτη γραμμή και στήλη του πίνακα
            else if (w[(i - 1)] <= wt)             // εάν το βάρος του προηγούμενου αντικειμένου είναι μικρότερο ή ίσο με τον αριθμό της στήλης
                K[i][wt] = max(v[(i - 1)] + K[i - 1][wt - w[(i - 1)]], K[i - 1][wt]); // υπολογίζουμε από την σχέση την τιμή του στοιχείου  του πίνακα
            else
                K[i][wt] = K[i - 1][wt];              // διαφορετικά το κάνουμε ίσο με το στοιχείο της προηγούμενης γραμμής της ίδια στήλης
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


    std::vector <item> solution;                    // δημιουργούμε το vector που θα περιέχει τα επιλεγμένα στη λύση αντικείμενα
    if (!isOutOfTime) {
        while (n != 0) {                            // το n περιέχει την αριθμό των αντικειμένων και στον πίνακα δείχνει στην τελευταία γραμμή
                                                    // τρέχουμε όλα τα στοιχεία του πίνακα (αντίστροφα) από την τελευταία στήλη και γραμμή προς την αρχή του
            if (K[n][W] != K[n - 1][W]) {           // ελέγχουμε αν το τρέχον στοιχείο του πίνακα είναι διαφορετικό από το προηγούμενο
                                                    // σε περίπτωση που είναι, το αντικείμενο αποτελεί μέρος της λύσης
                item sol = ks.items.at((n - 1));    // δημιουργούμε ένα struct sol για να αποθηκεύουμε το αντικείμενο που έχει επιλεχθεί στη λύση
                solution.push_back(sol);            // προσθέτουμε το αντικείμενο στη λίστα με τα επιλεγμένα αντικείναμε
                W = W - w[(n - 1)];                 // συνεχίζουμε τον έλεγχο από την προηγούμενη στήλη. Δηλαδή κάθε φορά που βρίσκουμε ένα μέρος της λύσης
                                                    // αλλάζουμε στήλη πηγαίνοντας μία πίσω
            }
            n--;                                    // ενώ αν δεν βρούμε μέρος της λύσης αλλάζουμε γραμμή πηγαίνοντας μία γραμμή πίσω μέχρι να ελέγξουμε και
                                                    // και την δεύτερη γραμμή του πίνακα (η πρώτη είναι μηδενισμένη)
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
    return solution;                            // επιστρέφουμε το vector solution το οποίο περιέχει τη λιστα με τα επιλεγμένα στη λύση αντικείμενα
}
/* print_solution
   Η print_solution δέχεται ως παράμετρο την λύση ενός προβλήματος knapsack_problem
   και εκτυπώνει τα στοιχεία του
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

   Η export_solution  αποθηκεύει τη λύση ενός knapsach προβλήματος σε ένα αρχείο
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
   Η export_csv βρίσκει τις λύσεις με όλους τους δυνατούς αλγόριθμους και εξάγει το αποτέλεσμα σε αρχείο CSV
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
    if (!fout.good()) {     // Ελέγχουμε αν δεν έχει δημιουργηθεί
        std::cerr << "--- Error opening file " << csv_path << std::endl;   // εκτυπώνουμε μήνυμα λάθους σε αυτή την περίπτωση
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
