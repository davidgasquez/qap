#include "common.h"
#include "greedy.h"
#include "input.h"
#include "localSearch.h"
#include "tabuSearch.h"
#include "memetic.h"

using namespace std;

int main (int argc, char *argv[])
{

    if (argc < 2 || argc > 3)
    {
        cerr << "ERROR: Execute with: qap [data]" << endl;
        return -1;
    }

    // Set seed
    int seed = 123456;

    // Read file
    Input input(argv[1]);
    if (!input.read())
    {
        cerr << "ERROR: Not file found";
        return -1;
    }

    cout << endl;

    // Tabu Search with 100000 evaluations
    srand(seed);
    TabuSearch ts(input);

    cout << "Tabu Search: " << endl;
    cout << "\tCost: " << ts.getCost() << endl;
    cout << "\tTime: " << ts.getTime() << endl;
    cout << "\tSolution: ";
    vector<int> solution =  ts.getSolution();
    for (int i = 0; i < solution.size(); ++i)
    {
        cout << solution[i] << " ";
    } cout << endl;

    cout << endl;

    // Stationary Genetic Algorythm with position based crossings with 100000 evaluations
    srand(seed);
    Memetic m(input, 100000, 20, 0.2);

    cout << "Memetic: " << endl;
    cout << "\tCost: " << m.getCost() << endl;
    cout << "\tTime: " << m.getTime() << endl;
    cout << "\tSolution: ";
    solution =  m.getSolution();
    for (int i = 0; i < solution.size(); ++i)
    {
        cout << solution[i] << " ";
    } cout << endl;

    cout << endl;
    return 0;
}