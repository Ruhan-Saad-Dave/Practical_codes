/*
Write a program in C++ to use map associative container. The keys will be the names of states and the values will be the populations of the states. When the program runs, the user is prompted to type the name of a state. The program then looks in the map, using the state name as an index and returns the population of the state.
*/

#include <iostream>
#include <map>
#include <string>

int main() {
    // Create a map to store state names and their populations
    std::map<std::string, int> statePopulation;

    // Initialize the map with some state populations
    statePopulation["California"] = 39538223;
    statePopulation["Texas"] = 29145505;
    statePopulation["Florida"] = 21538187;
    statePopulation["New York"] = 20201249;
    statePopulation["Pennsylvania"] = 13002700;

    std::string stateName;
    std::cout << "Enter the name of a state: ";
    std::getline(std::cin, stateName);

    // Look for the state in the map and display the population
    auto it = statePopulation.find(stateName);
    if (it != statePopulation.end()) {
        std::cout << "The population of " << stateName << " is " << it->second << "." << std::endl;
    } else {
        std::cout << "State not found." << std::endl;
    }

    return 0;
}

/*
Enter the name of a state: Texas
The population of Texas is 29145505.

Enter the name of a state: Florida
The population of Florida is 21538187.

Enter the name of a state: Ohio
State not found.
*/
