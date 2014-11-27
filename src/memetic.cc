#include "memetic.h"

Memetic::Memetic(const Input &input,  int maxEvaluations, int populationSize, double probabilityLS)
    : dimension_(input.getDimension()), distances_(input.getDistances()), flow_(input.getFlow()), maxEvaluations_ (maxEvaluations),
      populationSize_ (populationSize), evaluations_(0), probabilityLS_(probabilityLS)
{
    // Initialization
    touched_.resize(populationSize_, true);
    solution_.resize(dimension_);
    solution_ = randomSolution();
    cost_ = calculateCost(solution_);


    // Compute the crossovers based in expected value
    crossovers_ = populationSize_ / 2 * 0.7;


    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    time_ = time_span.count();
}

void Memetic::execute()
{
    // Initialize and evaluate
    createPopulation();
    evaluate(oldPopulation_);

    // Compute LS population;
    int populationLS = probabilityLS_ * populationSize_;

    while (evaluations_ <= maxEvaluations_)
    {
        // Take best father
        newPopulation_.push_back(oldPopulation_[0]);
        newPopulation_.resize(1);

        // Shuffle Population
        random_shuffle (oldPopulation_.begin(), oldPopulation_.end());

        // Breed
        breed(oldPopulation_);
        // Mutate
        mutate(oldPopulation_);

        // Evaluate the population
        evaluate(oldPopulation_);

        // Sort oldPopulation and add the first 49 to the newPopulation
        sort (oldPopulation_.begin(), oldPopulation_.end());

        // Aply local search to population after sorting
        if (evaluations_ % 10 == 0)
        {
            for (int i = 0; i < populationLS; ++i)
            {
                Input aa(dimension_, distances_, flow_);
                LocalSearch ls(aa, true);
                ls.setSolution(oldPopulation_[i].solution);
                ls.execute();

                // Add LS evaluations
                evaluations_ += 100;
                oldPopulation_[i].solution = ls.getSolution();
                oldPopulation_[i].cost = ls.getCost();
            }
        }

        for (int i = 1; i < populationSize_; ++i)
        {
            newPopulation_.push_back(oldPopulation_[i - 1]);
        }

        //Sort the final population and asign it to oldPopulation
        sort (newPopulation_.begin(), newPopulation_.end());
        oldPopulation_ = newPopulation_;

        // Reset the touched vector to false because we know all costs
        touched_.resize(populationSize_);
        fill(touched_.begin() , touched_.end(), false);
    }

    // Solution is the best solution of the population at the end of the execution
    solution_ = oldPopulation_[0].solution;
    cost_ = oldPopulation_[0].cost;
}

void Memetic::mutate(vector <Chromosome> &population)
{
    // Number of mutations;
    int mutations = dimension_ * populationSize_ * 0.01;

    for (int i = 0; i < mutations; ++i)
    {
        // Random chromosome and random gen
        int chromosome = rand() % population.size();
        int gen = rand() % dimension_;

        // Swap gen in chromosome
        swap(population[chromosome].solution, gen, rand() % dimension_);
        touched_[chromosome] = true;
    }
}

void Memetic::breed(vector <Chromosome> &population)
{

    // If they are uneven crossovers, add one.
    if (crossovers_ % 2 != 0)
    {
        crossovers_++;
    }

    // Create the crossovers vector of couples
    vector<Chromosome> couples;
    couples.resize(crossovers_);
    copy(population.begin(), population.begin() + crossovers_, couples.begin());

    // Cross couples
    for (int i = 0; i <= crossovers_ - 2; i += 2)
    {
        Chromosome son = cross(population[i], population[i + 1]);
        population.push_back(son);
        touched_.push_back(true);

        Chromosome brother = cross(population[i + 1], population[i]);
        population.push_back(brother);
        touched_.push_back(true);
    }
}

Chromosome Memetic::cross(Chromosome father, Chromosome mother)
{

    // Create son chromosome
    Chromosome son;
    vector<int> solution;
    solution.resize(dimension_, 0);
    vector<bool> assigned(dimension_, false);

    // Take from fathers and  assign common locations
    for (int i = 0; i < dimension_; ++i)
    {
        if (father.solution[i] == mother.solution[i])
        {
            solution[i] = father.solution[i];
            assigned[i] = true;
        }
    }

    // Get free locations
    vector<int> freeLocations;
    for (int i = 0; i < assigned.size(); i++)
    {
        if (!assigned[i])
        {
            freeLocations.push_back(father.solution[i]);
        }
    }

    // Suffle the free locations
    random_shuffle (freeLocations.begin(), freeLocations.end());

    // Assign the random locations to the son
    int pos = 0;
    for (int i = 0; i < freeLocations.size(); i++)
    {
        while (assigned[pos])
        {
            pos++;
        }
        solution[pos] = freeLocations[i];
        pos++;
    }

    // Add to son solution
    son.solution = solution;
    return son;
}

void Memetic::createPopulation()
{
    for (int i = 0; i < populationSize_; ++i)
    {
        // Add chromosome to the initial population
        Chromosome chromosome;
        chromosome.solution = randomSolution();
        oldPopulation_.push_back(chromosome);

        // Turn touched to true for later computation of the cost
        touched_[i] = true;
    }
}

void Memetic::evaluate(vector <Chromosome> &population)
{
    for (int i = 0; i < population.size(); ++i)
    {
        if (touched_[i])
        {
            // Get cost of the chromosome solution and update touched.
            population[i].cost = calculateCost(population[i].solution);
            touched_[i] = false;
        }
    }
}

void Memetic::swap(vector<int> &solution, int r, int s)
{
    int t;
    // Simple swap
    t = solution[r];
    solution[r] = solution[s];
    solution[s] = t;
}

vector<int> Memetic::randomSolution()
{
    vector<int> solution;
    solution.resize(dimension_);

    // Initialize
    for (int i = 0; i < dimension_; ++i)
    {
        solution[i] = i;
    }

    // Shuffle vector
    random_shuffle (solution.begin(), solution.end());

    return solution;
}

int Memetic::calculateCost(vector<int> &solution)
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

    // Add one evaluation
    evaluations_++;

    return cost;
}

vector<int> Memetic::getSolution()
{
    return solution_;
}

int Memetic::getCost()
{
    return cost_;
}

double Memetic::getTime()
{
    return time_;
}
