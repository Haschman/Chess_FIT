#include "Menu.hpp"
#include <iostream>


int main() {
    Menu main;

    try {
        main.initialize();
    } catch (const std::runtime_error &e) {
        std::cout << "\nYou can't play with this behaviour :(\n\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

