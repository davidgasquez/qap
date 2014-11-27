#ifndef COST_H__
#define COST_H__

#include "common.h"

class Cost
{
public:
    Cost(const string &filename);

    bool execute();

    string getFilename() const;
    int getDimension() const;

    vector< vector<int> > getDistances() const;
    vector< vector<int> > getFlow() const;

private:
    string filename_;
    int dimension_;
    vector<int> solution_;
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
};

#endif