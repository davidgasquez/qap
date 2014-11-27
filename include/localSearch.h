#ifndef LOCALSEARCH_H__
#define LOCALSEARCH_H__

#include "input.h"
#include "common.h"

using namespace std;
using namespace std::chrono;

class LocalSearch
{
public:
    LocalSearch(const Input &input);
    LocalSearch(const Input& input, bool noExecute);

    void execute();

    // Simple swap
    void swap(vector<int> &solution, int r, int s);
    // Random initial solution
    void randomStartSolution();
    // Factorization
    int moveCost(vector<int> &oldSolution, int r, int s);
    // Calculate cost using objetive function
    int calculateCost(vector<int> &solution);

    void setSolution(vector<int> solution);

    vector<int> getSolution();
    int getCost();
    double getTime();

private:
    // Input data
    int dimension_;
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;

    // Don't look bits mask
    vector<bool> dlb_;

    // Solution data
    vector<int> solution_;
    int cost_;
    double time_;
};

#endif