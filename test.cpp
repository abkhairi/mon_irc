#include <iostream>
#include <iomanip> // Nécessaire pour std::setfill et std::setw

int main() {
    int width = 30; // Largeur totale

    // Remplir avec un caractère personnalisé
    std::cout << std::setfill('-') << std::setw(width) << "-" << std::endl;

    // Utiliser std::setfill pour une ligne encadrée
    std::cout << "|" << std::setfill(' ') << std::setw(width - 2) << "Hello, World!" << "|" << std::endl;

    // Bordure inférieure
    std::cout << std::setfill('-') << std::setw(5) << "-" << std::endl;

    return 0;
}
