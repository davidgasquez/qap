#include "localSearch.h"

LocalSearch::LocalSearch(const Input &input)
    : dimension_(input.getDimension()), distances_(input.getDistances()), flow_(input.getFlow())
{
    solution_.resize(dimension_);
    dlb_.resize(dimension_, false);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    randomStartSolution();
    execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    time_ = time_span.count();
}

LocalSearch::LocalSearch(const Input &input, bool noExecute)
    : dimension_(input.getDimension()), distances_(input.getDistances()), flow_(input.getFlow())
{
    solution_.resize(dimension_);
    dlb_.resize(dimension_, false);
}

void LocalSearch::execute()
{
    // Cost of this solution
    cost_ = calculateCost(solution_);

    // Boolean for improvement
    bool improvement = true;
    int evaluations = 0;

    while (improvement  &&  evaluations < 100)
    {
        // Iterative Improvement
        for (int i = 0; i < dimension_ ; ++i)
        {

            if (dlb_[i] == false)
            {

                improvement = false;
                for (int j = 0; j < dimension_; ++j)
                {
                    // Get the moving cost
                    int movingCost = moveCost(solution_, i , j);

                    // If it's a better solution jump to it
                    if (movingCost < 0)
                    {
                        // Swap positions and get the cost
                        swap(solution_, i, j);
                        cost_ += movingCost;

                        // Update dlb mask
                        dlb_[i] = false;
                        dlb_[j] = false;

                        improvement = true;
                    }
                    evaluations++;
                }

                // Update dlb mask
                if (!improvement)
                {
                    dlb_[i] = true;
                }
            }
        }
    }
}

void LocalSearch::swap(vector<int> &solution, int r, int s)
{
    int t;
    // Simple swap
    t = solution[r];
    solution[r] = solution[s];
    solution[s] = t;
}

void LocalSearch::randomStartSolution()
{
    // Initialize
    for (int i = 0; i < dimension_; ++i)
    {
        solution_[i] = i;
    }

    // Shuffle vector
    random_shuffle (solution_.begin(), solution_.end());
}

void LocalSearch::setSolution(vector<int> solution)
{
    solution_ = solution;
}

int LocalSearch::calculateCost(vector<int> &solution)
{
    int cost = 0;
    for (int i = 0; i < dimension_; ++i)
    {
        for (int j = 0; j < dimension_; ++j)
        {
            // Objetive function
            cost += flow_[i][j] * distances_[solution[i]][solution[j]];
        }
    }

    return cost;
}


int LocalSearch::moveCost(vector<int> &oldSolution, int r, int s)
{
    int cost = 0;

    for (int k = 0; k < dimension_; ++k)
    {
        if (k != r && k != s)
        {
            cost +=
                flow_[r][k] * (distances_[oldSolution[s]][oldSolution[k]] - distances_[oldSolution[r]][oldSolution[k]]) +
                flow_[s][k] * (distances_[oldSolution[r]][oldSolution[k]] - distances_[oldSolution[s]][oldSolution[k]]) +
                flow_[k][r] * (distances_[oldSolution[k]][oldSolution[s]] - distances_[oldSolution[k]][oldSolution[r]]) +
                flow_[k][s] * (distances_[oldSolution[k]][oldSolution[r]] - distances_[oldSolution[k]][oldSolution[s]]);
        }
    }

    return cost;
}


vector<int> LocalSearch::getSolution()
{
    return solution_;
}

int LocalSearch::getCost()
{
    return cost_;
}

double LocalSearch::getTime()
{
    return time_;
}
