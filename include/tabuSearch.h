#ifndef TABUSEARCH_H__
#define TABUSEARCH_H__

#include "input.h"
#include "common.h"
#include <deque>

using namespace std;
using namespace std::chrono;

typedef struct
{
    int i, j, posI, posJ;
} TabuMove;

typedef struct Neighbour
{
    int r, s, cost;
    bool operator<(const Neighbour &n) const
    {
        return cost < n.cost;
    }
} Neighbour;

class TabuSearch
{
public:
    explicit TabuSearch(const Input &input, double neighbourhoodSize = 25, int evaluations = 100000, int maxFails = 20);

    void execute();

    // Simple swap
    void swap(vector<int> &solution, int r, int s);
    // Factorization
    int moveCost(vector<int> &oldSolution, int r, int s);
    // Calculate cost using objetive function
    int calculateCost(vector<int> &solution);

    // Tabu Search functions
    bool checkMove(int r, int s, vector<int> &currentSolution);

    // Reset strategies
    vector<int> diversify();
    vector<int> intensify();
    vector<int> useLTMemory();

    // Reset the tabu list
    void tabuReset(int &tabuLength);

    vector<int> getSolution();
    int getCost();
    double getTime();

private:
    // Input data
    int dimension_;
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;

    // Tabu Moves
    deque<TabuMove> tabuList_;

    // Neighbourhood
    vector<Neighbour> neighbourhood_;

    // Frecuency matrix
    vector<vector<int>> frec_;

    // Parameters
    int neighbourhoodSize_;
    int maxEvaluations_;
    int maxFails_;

    // Solution data
    vector<int> solution_;
    int cost_;
    double time_;
};

#endif