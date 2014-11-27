#include "input.h"

Input::Input(const string &filename)
    : filename_(filename), dimension_(0)
{

}

Input::Input(int dimension, vector< vector<int> > distances, vector< vector<int> > flow)
    : filename_("input")
{
    flow_ = flow;
    distances_ = distances;
    dimension_ = dimension;
}

bool Input::read()
{
    ifstream data(filename_.c_str());

    if (!data)
    {
        cerr << "ERROR: Can not open input data file: " << filename_ << endl;
        return false;
    }

    // Obtain dimension
    data >> dimension_;

    // Initialize distance and flow matrix
    distances_.resize(dimension_);
    for (int i = 0; i < dimension_; ++i)
    {
        distances_[i].resize(dimension_);
    }

    flow_.resize(dimension_);
    for (int i = 0; i < dimension_; ++i)
    {
        flow_[i].resize(dimension_);
    }

    // Fill distances and flow matrix
    for (int i = 0; i < dimension_; i++)
    {
        for (int j = 0; j < dimension_; ++j)
        {
            data >> distances_[i][j];
        }
    }

    for (int i = 0; i < dimension_; i++)
    {
        for (int j = 0; j < dimension_; ++j)
        {
            data >> flow_[i][j];
        }
    }

    return true;
}

string Input::getFilename() const
{
    return filename_;
}

int Input::getDimension() const
{
    return dimension_;
}

vector< vector<int> > Input::getDistances() const
{
    return distances_;
}

vector< vector<int> > Input::getFlow() const
{
    return flow_;
}