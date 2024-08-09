#ifndef COIN_H
#define COIN_H

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

#define DELIM ","  // delimiter

enum Denomination {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS, TWENTY_DOLLARS
};

class Coin {
public:
    enum Denomination denom;
    unsigned count;

    std::map<int, int> denominations;
    void loadDenominations(const std::string& filename);
    bool isValidDenomination(int denomination);
    bool canMakeChange(double amount);
    std::vector<std::pair<int, int>> makeChange(double& amount);
    void displayBalance() const;
    void saveDenominations(const std::string& filename) const;
};

#endif  // COIN_H