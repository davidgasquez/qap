#ifndef INPUT_H__
#define INPUT_H__

#include "common.h"

class Input
{
public:
    Input(const string &filename);
    Input(int dimension, vector< vector<int> > distances, vector< vector<int> > flow);

    bool read();

    string getFilename() const;
    int getDimension() const;

    vector< vector<int> > getDistances() const;
    vector< vector<int> > getFlow() const;



private:
    string filename_;
    int dimension_;
    vector< vector<int> > distances_;
    vector< vector<int> > flow_;
};

#endif