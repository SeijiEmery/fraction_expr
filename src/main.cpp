
#include "fraction.hpp"

void repl () {
    std::string s;
    while (1) {
        std::cout << ">> ";
        while (!getline(std::cin, s)) {}
        std::cout << "   " << Fraction { s } << '\n';
    }
}

int main (int argc, const char** argv) {
    Fraction::unittest();
    repl();
}
