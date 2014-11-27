#include "cost.h"

Cost::Cost(const string &filename)
    : filename_(filename), dimension_(0)
{

}

bool Cost::execute()
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

    // Get the solution
    solution_.resize(dimension_);
    for (int i = 0; i < dimension_; ++i)
    {
        data >> solution_[i];
    }

    // Calculate cost
    int cost = 0;
    for (int i = 0; i < dimension_; ++i)
    {
        for (int j = 0; j < dimension_; ++j)
        {
            // Objetive function
            cost += flow_[i][j] *  distances_[solution_[i]][solution_[j]];
        }
    }

    cout << "\tCost: " << cost << endl;

    return true;
}

string Cost::getFilename() const
{
    return filename_;
}

int Cost::getDimension() const
{
    return dimension_;
}

vector< vector<int> > Cost::getDistances() const
{
    return distances_;
}

vector< vector<int> > Cost::getFlow() const
{
    return flow_;
}