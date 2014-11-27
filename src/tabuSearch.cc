#include "tabuSearch.h"
#include "localSearch.h"

TabuSearch::TabuSearch(const Input &input, double neighbourhoodSize, int maxEvaluations, int maxFails)
    : dimension_(input.getDimension()), distances_(input.getDistances()), flow_(input.getFlow())
{
    solution_.resize(dimension_);

    // Fix parameters
    neighbourhoodSize_ = neighbourhoodSize;
    maxEvaluations_ = maxEvaluations;
    maxFails_ = maxFails;

    //Initialize frecuency matrix
    frec_.resize(dimension_);
    for (int i = 0; i < frec_.size(); ++i)
    {
        frec_[i].resize(dimension_);
    }

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    time_ = time_span.count();

}

void TabuSearch::execute()
{

    // Inner parameters
    int tabuLength = dimension_ / 5;
    int fails = 0;
    bool improvement = false;

    // Initialize
    for (int i = 0; i < dimension_; ++i)
    {
        solution_[i] = i;
    }

    // Shuffle vector
    random_shuffle(solution_.begin(), solution_.end());

    // Get cost
    cost_ = calculateCost(solution_);

    // Current solution and cost
    vector<int>currentSolution;
    currentSolution = solution_;
    int currentCost = cost_;

    int evaluations = 0;

    while (evaluations < maxEvaluations_)
    {
        improvement = false;

        // Empty the neighborhoud
        neighbourhood_.clear();

        // Generate the Neighbourhood
        for (int i = 0; i < neighbourhoodSize_; ++i)
        {
            // Generate random r and s
            int r = rand() % dimension_;
            int s = rand() % dimension_;

            // Check the difference
            int diff = moveCost(currentSolution, r, s);
            evaluations++;

            // Add to the neighbourhood
            Neighbour neighbor;
            neighbor.r = r;
            neighbor.s = s;
            //neighbor.cost = diff + currentCost;
            neighbor.cost = diff + currentCost;

            neighbourhood_.push_back(neighbor);
        }

        // Sort the neighbourhood
        sort(neighbourhood_.begin(), neighbourhood_.end());

        // See the neighbourhood
        bool selected = false;
        int i = 0;

        // Check if the move is not Tabu
        selected = checkMove(neighbourhood_[i].r, neighbourhood_[i].s, currentSolution);

        // Check if is better than our solution
        int nCost = currentCost + moveCost(currentSolution, neighbourhood_[i].r, neighbourhood_[i].s);
        if (!selected && nCost < cost_)
        {
            selected = true;
        }
        i++;

        while (!selected && i < neighbourhoodSize_)
        {
            // Check if the move is not Tabu
            selected = checkMove(neighbourhood_[i].r, neighbourhood_[i].s, currentSolution);
            i++;
        }

        // If none of the neighbourhood is selected, generate new neighbourhood
        if (!selected)
        {
            continue;
        }

        int r =  neighbourhood_[i - 1].r;
        int s =  neighbourhood_[i - 1].s;

        // Jump to something
        swap(currentSolution, r, s);
        currentCost = neighbourhood_[i - 1].cost;

        // Update frecuency matrix
        frec_[r][currentSolution[r]]++;
        frec_[s][currentSolution[s]]++;

        if (currentCost < cost_)
        {
            improvement = true;
            solution_ = currentSolution;
            cost_ = currentCost;
        }

        // Create tabu move
        TabuMove tabuMove;
        tabuMove.i = currentSolution[neighbourhood_[i - 1].r];
        tabuMove.j = currentSolution[neighbourhood_[i - 1].s];
        tabuMove.posI = neighbourhood_[i - 1].r;
        tabuMove.posJ = neighbourhood_[i - 1].s;

        // Add tabu move
        tabuList_.push_back(tabuMove);

        //Size of the deque
        if (tabuList_.size() > tabuLength)
        {
            tabuList_.pop_front();
        }

        // Count  when not improvement
        if (improvement)
        {
            fails = 0;
        }
        else
        {
            fails ++;
            if (fails >= maxFails_)
            {
                // Random (0,1)
                float random = ((double) rand() / (RAND_MAX));
                // Resets
                if (random < 0.25)
                {
                    currentSolution = diversify();
                    currentCost = calculateCost(currentSolution);
                    tabuReset(tabuLength);
                }
                else if (random < 0.6)
                {
                    currentSolution = intensify();
                    currentCost = calculateCost(currentSolution);
                    tabuReset(tabuLength);
                }
                else
                {
                    currentSolution = useLTMemory();
                    currentCost = calculateCost(currentSolution);
                    tabuReset(tabuLength);
                }
            }
        }
    }
}

void TabuSearch::swap(vector<int> &solution, int r, int s)
{
    int t;
    // Simple swap
    t = solution[r];
    solution[r] = solution[s];
    solution[s] = t;
}

int TabuSearch::moveCost(vector<int> &oldSolution, int r, int s)
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

int TabuSearch::calculateCost(vector<int> &solution)
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

// Search in the Tabu Moves if the move r,s is allowed with the vector current solution
bool TabuSearch::checkMove(int r, int s, vector<int> &currentSolution)
{
    for (int n = 0; n < tabuList_.size(); ++n)
    {
        if ((tabuList_[n].i == r && tabuList_[n].posI == currentSolution[r] ))
        {
            return false;
        }

        if ((tabuList_[n].i == s && tabuList_[n].posI == currentSolution[s] ))
        {
            return false;
        }

        if ((tabuList_[n].j == r && tabuList_[n].posJ == currentSolution[s] ))
        {
            return false;
        }

        if ((tabuList_[n].j == s && tabuList_[n].posJ == currentSolution[s] ))
        {
            return false;
        }
    }

    return true;
}

vector<int> TabuSearch::diversify()
{
    vector<int> newSolution(dimension_);
    // Initialize
    for (int i = 0; i < dimension_; ++i)
    {
        newSolution[i] = i;
    }

    // Shuffle vector
    random_shuffle (newSolution.begin(), newSolution.end());

    return newSolution;
}

void TabuSearch::tabuReset(int &tabuLength)
{
    // Clear Tabu List and change tabu size
    tabuList_.clear();

    float random = ((double) rand() / (RAND_MAX));
    if (random < 0.5)
    {
        tabuLength = tabuLength + (0.5 * tabuLength);
    }
    else
    {
        tabuLength *= random - (0.5 * tabuLength);
    }
}

vector<int> TabuSearch::intensify()
{
    // Return best solution
    return solution_;
}

vector<int> TabuSearch::useLTMemory()
{
    vector<int> newSolution(dimension_);
    vector<int> remain(dimension_);

    // Initialize
    for (int i = 0; i < dimension_; ++i)
    {
        remain[i] = i;
    }

    // Obtain min values from frecuency matrix
    for (int i = 0; i < dimension_; ++i)
    {
        int min_value = INT_MAX;
        int min_index = 0;
        int iterator = 0;

        for (int j = 0; j < remain.size(); ++j)
        {
            if (frec_[i][j] < min_value)
            {
                min_value = frec_[i][j];
                min_index = remain[j];
                iterator = j;
            }
        }

        // Asign the index of min value and remove it from remainings index
        newSolution[i] = min_index;
        remain.erase(remain.begin() + iterator);
    }

    return newSolution;

}

vector<int> TabuSearch::getSolution()
{
    return solution_;
}

int TabuSearch::getCost()
{
    return cost_;
}

double TabuSearch::getTime()
{
    return time_;
}

