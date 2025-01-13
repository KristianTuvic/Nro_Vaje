#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <ctime>

using namespace std;

int main() {

    // inicializiramo matriko A
    vector<vector<double>> A;

    // inicializiramo vektor b;
    vector<double> b;

    
    std::string filename = "./datoteka_A_b.txt";

    
    std::ifstream infile;
    infile.open(filename);
    // Preverimo, ali je datoteka uspešno odprta
    if (!infile.is_open()) {
        std::cerr << "Napaka pri odpiranju datoteke!" << std::endl;
        return -1;
    }

    
    std::string string_first_line;
    std::getline(infile, string_first_line);

    
    
    std::replace(string_first_line.begin(), string_first_line.end(), '=', ' ');

    
    std::istringstream iss(string_first_line);
    std::string nepomemben_del1; 
    std::string nepomemben_del2; 
    int n;

    
    iss >> nepomemben_del1;
    iss >> nepomemben_del2;
    iss >> n;

    std::cout << "Velikost matrike A: " << n << "x" << n << std::endl;;

   
    for (int iiA = 0; iiA < n; iiA++)
    {
        
        std::string line;
        std::getline(infile, line);
        
        std::replace(line.begin(), line.end(), ';', ' ');

       
        std::istringstream iss_column(line);

        
        vector<double> row;

        // sedaj lahko iteriramo po elementih v iss_column
        for (int column = 0; column < n; column++)
        {
            double element_a = 0;
            iss_column >> element_a;
            row.push_back(element_a);
        }

        
        A.push_back(row);
    }

    // sedaj imamo sestavljeno matriko A. Naslednja vrstica je prazna,
    // zato jo samo preberemo in ne naredimo nicesar.
    
    std::string empty_line;
    std::getline(infile, empty_line);

    // prebrati moramo se vektor b
    std::string string_line_b;
    std::getline(infile, string_line_b);

    
    std::replace(string_line_b.begin(), string_line_b.end(), '>', ' ');
    std::istringstream iss_b(string_line_b);
    int n_b; 

    iss_b >> nepomemben_del1;
    iss_b >> nepomemben_del2;
    iss_b >> n_b;

    std::cout << "Velikost vektorja b: " << n_b << std::endl;;

    // naredimo iteracijo po naslednjem n_b stevilu vrstic
    for (int iib = 0; iib < n_b; iib++)
    {
        // preberemo vrstico in shranimo element v vrstici v vektor b
        std::string line_b_element;
        std::getline(infile, line_b_element);
        std::istringstream iss_b_element(line_b_element);

        double b_element = 0;
        iss_b_element >> b_element;

        b.push_back(b_element);
    }

    // Sedaj imamo A in b. Lahko napisemo Gauss-Seidel metodo. Najprej
    // inicializiramo vektor resitve T, npr. na 100 stopinj.
    vector<double> T;
    for (int iiT = 0; iiT < n_b; iiT++)
    {
        T.push_back(100);
    }

    // Uporabnik lahko izbere naèin izvajanja
    bool use_parallel = false; // Nastavite na true za moznost s paralelizacijo

    auto start_time = std::chrono::high_resolution_clock::now();

    if (use_parallel) {
        // Paralelna izvedba z OpenMP
        for (int ii = 0; ii < 2000; ii++) {
#pragma omp parallel for shared(T, A, b)
            for (int jj = 0; jj < n; jj++) {
                double d = b[jj];
                for (int kk = 0; kk < n; kk++) {
                    if (jj != kk) {
                        d -= A[jj][kk] * T[kk];
                    }
                }
#pragma omp critical
                T[jj] = d / A[jj][jj];
            }
        }
    }
    else {
        // Brez paralelizacije
        for (int ii = 0; ii < 2000; ii++) {
            for (int jj = 0; jj < n; jj++) {
                double d = b[jj];
                for (int kk = 0; kk < n; kk++) {
                    if (jj != kk) {
                        d -= A[jj][kk] * T[kk];
                    }
                }
                T[jj] = d / A[jj][jj];
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_duration = end_time - start_time;
    std::cout << "Time of Gauss-Seidel: " << time_duration.count() << " seconds" << std::endl;

    // Za izpis maksimalne vrednosti
    double max_T = 0;
    for (int iiT = 0; iiT < n_b; iiT++)
    {
        if (T[iiT] > max_T)
        {
            max_T = T[iiT];
        }
    }

    cout << "Max. temperature: " << max_T << " degree C." << endl;

    return 0;
}