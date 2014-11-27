#ifndef GREEDY_H__
#define GREEDY_H__

#include "input.h"
#include "common.h"

using namespace std;
using namespace std::chrono;

class Greedy
{
public:
    Greedy(const Input &input);

    void execute();

    vector<int> getSolution();
    int getCost();
    double getTime();


private:
    int dimension_;
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;

    vector<int> solution_;

    vector<int> flowPotential_;
    vector<int> distancesPotential_;

    double time_;
};

#endif