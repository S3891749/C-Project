#include "Coin.h"
#include "Node.h"
#include <iostream>
#include <iomanip>  // For setw and left manipulators 

// This method loads coin denominations and their quantities from a file.
// It reads each line from the specified file, parses the denomination and quantity,
// and stores them in the denominations map. If a line cannot be parsed, it prints an error message.
void Coin::loadDenominations(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int denom, qty;
    char comma;
    while (getline(file, line)) {
        std::istringstream iss(line);
        if (iss >> denom >> comma >> qty) {
            denominations[denom] = qty;
        } else {
            std::cerr << "Failed to parse line: " << line << std::endl;
        }
    }
    file.close();
}

// This method checks if a given denomination is valid.
// It returns true if the denomination exists in the denominations map, otherwise false.
bool Coin::isValidDenomination(int denomination) {
    return denominations.find(denomination) != denominations.end();
}

// This method checks if it's possible to make change for a given amount.
// It converts the amount from dollars to cents and uses a temporary copy of the denominations map.
// It iterates over the denominations from largest to smallest, subtracting the value of each coin
// from the required amount until the amount is zero or no more coins are available.
// It returns true if the exact change can be made, otherwise false.
bool Coin::canMakeChange(double amount) {
    int cents_needed = static_cast<int>(amount * 100);
    std::map<int, int> temp_denoms = denominations;
    bool can_make_change = false;

    auto it = temp_denoms.rbegin();
    while (it != temp_denoms.rend()) {
        int coin_value = it->first;
        while (cents_needed >= coin_value && it->second > 0) {
            cents_needed -= coin_value;
            it->second--;
        }
        ++it;
    }

    if (cents_needed == 0) {
        can_make_change = true;
    }

    return can_make_change;
}

// This method attempts to make change for a given amount using available denominations.
// It converts the amount from dollars to cents and uses a temporary copy of the denominations map for simulation.
// It iterates over the denominations from largest to smallest, deducting the value of each coin from the required amount
// until the amount is zero or no more coins are available. If exact change can be made, it updates the actual denominations.
// It returns a vector of pairs, each containing a denomination and the number of coins used for that denomination.
std::vector<std::pair<int, int>> Coin::makeChange(double& amount) {
    int cents_needed = static_cast<int>(amount * 100);
    std::vector<std::pair<int, int>> change;
    std::map<int, int> temp_denoms = denominations;

    for (auto it = temp_denoms.rbegin(); it != temp_denoms.rend(); ++it) {
        int coin_value = it->first;
        int coins_used = 0;
        bool complete = false;
        
        while (cents_needed >= coin_value && it->second > 0 && !complete) {
            cents_needed -= coin_value;
            it->second--;
            coins_used++;
            if (cents_needed == 0) {
                complete = true;
            }
        }
        if (coins_used > 0) {
            change.push_back({coin_value, coins_used});
        }
        if (cents_needed == 0) {
            complete = true;
        }
    }

    if (cents_needed == 0) {
        for (const auto& ch : change) {
            denominations[ch.first] -= ch.second;
        }
    } else {
        change.clear();
    }
    return change;
}

// This method displays the current balance of all denominations in the system.
// It iterates over the denominations map and prints each denomination, the quantity, and the total value.
// It also calculates and prints the total value of all denominations combined.
void Coin::displayBalance() const {
    double totalValue = 0.0;
   std::cout << "Balance Summary\n";
    std::cout << "-------------\n";
    std::cout << "Denom | Quantity | Value\n";
    std::cout << "--------------------------\n";

    for (const auto& denom : denominations) {
        int denomination = denom.first;
        int quantity = denom.second;
        double value = denomination * quantity / 100.0;
        totalValue += value;

        std::ostringstream line;
        line << std::left << std::setw(5) << denomination << " | "
             << std::left << std::setw(8) << quantity << " |$ "
             << std::right << std::setw(6) << std::fixed << std::setprecision(2) << value;

        std::cout << line.str() << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
    std::cout << "                  $ " << std::fixed << std::setprecision(2) << totalValue << "\n";
}

// This method saves the current denominations and their quantities to a file.
// It opens the specified file for writing and writes each denomination and its quantity, separated by a delimiter.
// If the file cannot be opened, it prints an error message.
void Coin::saveDenominations(const std::string& filename) const {
    std::ofstream file(filename);
    bool file_opened = file.is_open();

    if (file_opened) {
        for (const auto& denom : denominations) {
            file << denom.first << DELIM << denom.second << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    }
}