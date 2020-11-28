#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <set>

//including files with realizations of trees
//#include ""
//#include ""

uint64_t p = 995009;
int a, b, c;
int range = 100;
int i, j, k, N;
int min_k = 10;
int max_k = 19;

auto start = std::chrono::high_resolution_clock::now();
auto finish = std::chrono::high_resolution_clock::now();
double interval = double(std::chrono::duration_cast <std::chrono::microseconds>(finish-start).count());

template <class T>
class Profiler
{
public:
    Profiler(){
        open_files();
        for (a = 1 ; a < range ; a++){
            check_asymptotics();
        }
        close_files();
    }
    ~Profiler(){

    }
private:
    T tested_tree;
    std::ofstream file3, file1, file2;

    check_asymptotics()
    {
        //just filling the beginning of the tree
        //auto u = tested_tree.insert(a % p);
        for (i = 0 ; i < pow(2, min_k) ; i++){
            tested_tree.insert(a*i % p);
        }

        //continue filling with recording
        for (k = min_k ; k < max_k ; k++){
            start = std::chrono::high_resolution_clock::now();
            for (j = pow(2, k) ; j < pow(2, k+1) ; j++){
                tested_tree.insert(a*j % p);
            }
            finish = std::chrono::high_resolution_clock::now();
            interval = double(std::chrono::duration_cast <std::chrono::microseconds>(finish-start).count());
            file3 << k << ',' << interval/(pow(2, k)) << '\n';
        }

        for (i = pow(2, max_k) ; i < p ; i++){
            tested_tree.insert(a*j % p);
        }

        auto it = tested_tree.find(2);

        for (k = max_k ; k > min_k ; k--){
            start = std::chrono::high_resolution_clock::now();
            for (j = pow(2, k) ; j < pow(2, k+1) ; j++){
                it = tested_tree.find((a*2)*j % p);
            }
            finish = std::chrono::high_resolution_clock::now();
            interval = double(std::chrono::duration_cast <std::chrono::microseconds>(finish-start).count());
            file1 << k << ',' << interval/(pow(2, k)) << '\n';

            start = std::chrono::high_resolution_clock::now();
            for (j = pow(2, k) ; j < pow(2, k+1) ; j++){
                tested_tree.erase((a*3)*j % p);
            }
            finish = std::chrono::high_resolution_clock::now();
            interval = double(std::chrono::duration_cast <std::chrono::microseconds>(finish-start).count());
            file2 << k << ',' << interval/(pow(2, k)) << '\n';
        }
    }

    void open_files()
    {
        file3.open("insert.csv");
        file1.open("find.csv");
        file2.open("erase.csv");
        file1 << "logN"  << ',' << "t" << '\n';
        file2 << "logN"  << ',' << "t" << '\n';
        file3 << "logN"  << ',' << "t" << '\n';
    }

    void close_files()
    {
        file1.close();
        file2.close();
        file3.close();
    }
};


int main()
{

    Profiler<std::set<int>> set_profiler;


    //Profiler<AVL<uint64_t>> AVL_profiler;

    //Profiler<RB<uint64_t>> RB_profiler;

}
