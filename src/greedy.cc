#include "greedy.h"

Greedy::Greedy(const Input &input)
    : dimension_(input.getDimension()), distances_(input.getDistances()), flow_(input.getFlow())
{
    solution_.resize(dimension_);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    time_ = time_span.count();
}

void Greedy::execute()
{
    // Calculate distances and flow potential
    for (int i = 0; i < dimension_; ++i)
    {
        int potential = 0;
        for (int j = 0; j < dimension_; ++j)
        {
            if (i != j)
            {
                potential += distances_[i][j];
            }
        }
        distancesPotential_.push_back(potential);
    }

    for (int i = 0; i < dimension_; ++i)
    {
        int potential = 0;
        for (int j = 0; j < dimension_; ++j)
        {
            if (i != j)
            {
                potential += flow_[i][j];
            }
        }
        flowPotential_.push_back(potential);
    }

    // Compute the solution
    for (int j = 0; j < dimension_; ++j)
    {
        int maxPos = 0;
        int minPos = 0;
        for (int i = 0; i < dimension_; ++i)
        {
            // Maximum distance potential
            if (distancesPotential_[i] > distancesPotential_[maxPos])
            {
                maxPos = i;
            }
            // Minimum flow potential
            if (flowPotential_[i] < flowPotential_[minPos])
            {
                minPos = i;
            }
        }
        solution_[maxPos] = minPos;
        distancesPotential_[maxPos] = -1;
        flowPotential_[minPos] = INT_MAX;
    }
}

vector<int> Greedy::getSolution()
{
    return solution_;
}

int Greedy::getCost()
{
    int cost = 0;
    for (int i = 0; i < dimension_; ++i)
    {
        for (int j = 0; j < dimension_; ++j)
        {
            // Objetive function
            cost += flow_[i][j] *  distances_[solution_[i]][solution_[j]];
        }
    }

    return cost;
}

double Greedy::getTime()
{
    return time_;
}