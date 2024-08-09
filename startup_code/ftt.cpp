#include <iostream>
#include <cstdlib>  // For EXIT_SUCCESS and EXIT_FAILURE
#include "LinkedList.h"
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include "Food.h"
#include "Coin.h"
using std::string;

// This function gets the highest existing food item ID in the menu list.
// It iterates through the linked list, checks the ID of each item, and updates the highest ID found.
int getHighestExistingID(const LinkedList& menuList) {
    int highestID = 0;
    Node* current = menuList.getHead();
    while (current != nullptr) {
        std::string id = current->data->id;
        if (id[0] == 'F') {
            int num = std::stoi(id.substr(1));
            if (num > highestID) {
                highestID = num;
            }
        }
        current = current->next;
    }
    return highestID;
}

// This function generates the next available food item ID by incrementing the highest existing ID.
std::string generateNextFoodID(const LinkedList& menuList, int& foodCounter) {
    if (foodCounter == 0) { // Initialize once
        foodCounter = getHighestExistingID(menuList);
    }
    std::ostringstream ss;
    ss << 'F' << std::setw(4) << std::setfill('0') << (foodCounter + 1);
    return ss.str();
}

// This function adds a new food item to the menu list.
// It prompts the user for the food item's details, generates a new ID, creates the item, and inserts it into the list.
void addFoodItem(LinkedList& menuList) {
    std::string itemName, itemDescription, itemPriceStr;
    double itemPrice;
    const unsigned defaultOnHand = 20;
    int foodCounter = 0;

    bool validInput = false;
    bool userCancelled = false;
    std::string itemID = generateNextFoodID(menuList, foodCounter);

    while (!validInput && !userCancelled) {
        std::cout << "This new meal item will have the Item ID of " << itemID << "." << std::endl;
        std::cout << "Enter the item name: ";
        if (!std::getline(std::cin, itemName) || itemName.empty()) {
            if (std::cin.eof() || itemName.empty()) {
                std::cin.clear();
                std::cout << "Returning to main menu." << std::endl;
                userCancelled = true;
            }
        } else {
            validInput = true;
        }
    }

    if (!userCancelled) {
        validInput = false;
        while (!validInput && !userCancelled) {
            std::cout << "Enter the item description: ";
            if (!std::getline(std::cin, itemDescription) || itemDescription.empty()) {
                if (std::cin.eof() || itemDescription.empty()) {
                    std::cin.clear();
                    std::cout << "Returning to main menu." << std::endl;
                    userCancelled = true;
                }
            } else {
                validInput = true;
            }
        }
    }

    if (!userCancelled) {
        validInput = false;
        while (!validInput && !userCancelled) {
            std::cout << "Enter the price for this item (in dollars and cents): ";
            if (!std::getline(std::cin, itemPriceStr) || itemPriceStr.empty()) {
                if (std::cin.eof() || itemPriceStr.empty()) {
                    std::cin.clear();
                    std::cout << "Returning to main menu and deleting the item with ID " << itemID << "." << std::endl;
                    userCancelled = true;
                }
            } else {
                std::istringstream iss(itemPriceStr);
                if (iss >> itemPrice && itemPrice >= 0) {
                    validInput = true;
                } else {
                    std::cout << "Invalid input. Please enter a valid price in dollars and cents (non-negative value)." << std::endl;
                }
            }
        }
    }

    if (!userCancelled) {
        Food newItem(itemID, itemName, itemDescription, itemPrice, defaultOnHand);
        menuList.insertNode(newItem);
        foodCounter++;
        std::cout << "This item \"" << itemName << " - " << itemDescription
                << "\" has now been added to the food menu." << std::endl;
    } else {
        std::cout << "No item was added to the menu." << std::endl;
    }
}


// This function removes a food item from the menu list.
// It prompts the user for the food item's ID and removes the item if it exists.
void removeFoodItem(LinkedList& menuList) {
    std::string itemID;
    bool continueRemoveItem = true;
    int attempts = 0;
    const int maxAttempts = 5;

    while(continueRemoveItem && attempts < maxAttempts) {
        std::cout << "Enter the food ID of the item to remove from the menu: ";
        std::getline(std::cin, itemID);

        if(itemID.empty()){
            std::cout << "Returning to main menu." << std::endl;
            continueRemoveItem = false;
        }
        else if (menuList.removeItem(itemID)){
            continueRemoveItem = false;
        }
        else{
            std::cout << "Failed to remove item with ID " << itemID << ". Item may not exist." << std::endl;
            attempts++;
            if (attempts >= maxAttempts) {
                std::cout << "Too many failed attempts. Returning to main menu." << std::endl;
                continueRemoveItem = false;
            }
        }
    }
}


// This function formats and prints the change given to the customer.
// It converts cents to dollars and cents and prints the change in a human-readable format.
void printChange(const std::vector<std::pair<int, int>>& change) {
    std::ostringstream changeOutput;
    for (const auto& c : change) {
        if (c.first >= 100) {
            if (c.first == 100) {
                changeOutput << "$1 ";
            } else {
                changeOutput << '$' << c.first / 100 << ' ';
            }
        } else {
            changeOutput << c.first << 'c' << ' ';
        }
    }
    std::cout << "Your change is " << changeOutput.str() << std::endl;
}

// This function handles the purchasing of a meal.
// It prompts the user for the food item ID, processes payment, provides change, and handles cancellation.
void purchaseMeal(LinkedList& menu, Coin& coins) {
    bool continueFindItem = true;
    bool validItemFound = false;
    bool userCancelled = false;
    std::string itemId;

    while (continueFindItem && !userCancelled) {
        std::cout << "Purchase Meal" << std::endl;
        std::cout << "-------------" << std::endl;
        std::cout << "Please enter the ID of the food you wish to purchase:" << std::endl;
        std::getline(std::cin, itemId);

        if (itemId.empty()) {
            std::cout << "Returning to main menu." << std::endl;
            userCancelled = true;
        } else {
            Node* itemNode = menu.findItem(itemId);
            if (itemNode == nullptr || !std::cin) {
                std::cerr << "Error: Item not found in menu." << std::endl;
            } else {
                validItemFound = true;
                continueFindItem = false;
            }
        }
    }

    if (validItemFound && !userCancelled) {
        Node* itemNode = menu.findItem(itemId); // Find the item again as continueFindItem is set to false
        FoodItem* selectedItem = itemNode->data;
        std::cout << "You have selected \"" << selectedItem->name << " - " << selectedItem->description << "\". This will cost you $" << selectedItem->price.dollars << '.' << std::setfill('0') << std::setw(2) << selectedItem->price.cents << std::endl;

        double totalPrice = selectedItem->price.dollars + (selectedItem->price.cents / 100.0);
        double paidAmount = 0.0;
        double remainingAmount = totalPrice;

        std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
        std::cout << "Please enter ctrl-D or enter on a new line to cancel this purchase." << std::endl;

        bool continuePurchase = true;
        while (continuePurchase && !userCancelled) {
            std::cout << "You still need to give us $" << std::fixed << std::setprecision(2) << remainingAmount << ": ";
            std::string paymentInput;
            std::getline(std::cin, paymentInput);

            if (paymentInput.empty()) {
                if (paidAmount > 0) {
                    std::cout << "Transaction canceled. Refunding all payments." << std::endl;
                    std::cout << "Refunded: $" << std::fixed << std::setprecision(2) << paidAmount << std::endl;
                }
                std::cout << "Returning to main menu." << std::endl;
                userCancelled = true;
            } else {
                std::istringstream iss(paymentInput);
                int payment;
                if (!(iss >> payment) || !coins.isValidDenomination(payment)) {
                    std::cout << "Error: invalid denomination encountered." << std::endl;
                } else {
                    if (coins.denominations[static_cast<int>(payment)] > 0) {
                        paidAmount += payment / 100.0;
                        remainingAmount = totalPrice - paidAmount;

                        if (remainingAmount < 0) {
                            double changeNeeded = -remainingAmount;
                            if (!coins.canMakeChange(changeNeeded)) {
                                std::cout << "Unable to provide correct change. Transaction cannot be completed." << std::endl;
                                std::cout << "Returning to main menu." << std::endl;
                                userCancelled = true;
                            } else {
                                std::vector<std::pair<int, int>> change = coins.makeChange(changeNeeded);
                                if (change.empty()) {
                                    std::cout << "Error: Unable to make the necessary change." << std::endl;
                                    std::cout << "Returning to main menu." << std::endl;
                                    userCancelled = true;
                                } else {
                                    printChange(change);
                                    continuePurchase = false;
                                }
                            }
                        } else if (remainingAmount == 0) {
                            std::cout << "Thank you for your payment!" << std::endl;
                            continuePurchase = false;
                        }
                    } else {
                        std::cout << "Error: Not enough coins of denomination to provide change." << std::endl;
                    }
                }
            }
        }
    }

    if (userCancelled) {
        std::cout << "Purchase process was cancelled." << std::endl;
    }
}

bool isValidMenuInput(const std::string& input) {
    // Check if input is a valid integer within the range 1-7
    bool valid = true;
    if (input.empty() || input.find_first_not_of("0123456789") != std::string::npos) {
        valid = false;
    } else {
        try {
            int option = std::stoi(input);
            if (option < 1 || option > 7) {
                valid = false;
            }
        } catch (const std::invalid_argument& ia) {
            valid = false;
        } catch (const std::out_of_range& oor) {
            valid = false;
        }
    }
    return valid;
}


// The main function initializes the program, loads data, displays the menu, and handles user input.
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <foodsfile> <coinsfile>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Food file: " << argv[1] << std::endl;
    std::cout << "Coins file: " << argv[2] << std::endl;

    LinkedList menuList;
    menuList.loadMenuFromFile(argv[1]);

    Coin coins;
    coins.loadDenominations(argv[2]);

    bool quit = false;
    std::string menuInput;

    while (!quit) {
        std::cout << "Main Menu:" << std::endl;
        std::cout << "  1. Display Meal Options" << std::endl;
        std::cout << "  2. Purchase Meal" << std::endl;
        std::cout << "  3. Save and Exit" << std::endl;
        std::cout << "Administrator-Only Menu:" << std::endl;
        std::cout << "  4. Add Food" << std::endl;
        std::cout << "  5. Remove Food" << std::endl;
        std::cout << "  6. Display Balance" << std::endl;
        std::cout << "  7. Abort Program" << std::endl;
        std::cout << "Select your option (1-7) :" << std::endl;
        std::getline(std::cin, menuInput);

        try {
            int option = std::stoi(menuInput);
            if (option == 1) {
                menuList.displayMenu();
            } else if (option == 2) {
                purchaseMeal(menuList, coins);
            } else if (option == 3) {
                menuList.saveMenuToFile(argv[1]);
                coins.saveDenominations(argv[2]);
                quit = true;
            } else if (option == 4) {
                addFoodItem(menuList);
            } else if (option == 5) {
                removeFoodItem(menuList);
            } else if (option == 6) {
                coins.displayBalance();
            } else if (option == 7) {
                std::cout << "Program is terminating. All program data will be lost." << std::endl;
                menuList.~LinkedList();
                coins.~Coin();
                exit(0);
            } else {
                std::cout << "Invalid input. Please try again." << std::endl;
            }
        } catch (const std::invalid_argument& ia) {
            std::cout << "Invalid input. Please enter a number from 1 to 7." << std::endl;
        }
    }
    std::cout << "Good bye!";
    return EXIT_SUCCESS;
}