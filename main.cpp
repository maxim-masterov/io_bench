#include <iostream>
#include <cstdint>

#include "Vector.h"
#include "IOManager.h"

int main(int argv, char **argc) {
    Vector<double> vec_in, vec_out;
    size_t vec_size = 100;
    Results results_wb, results_wa;
    Results results_rb, results_ra;
    IOManager io_manager("test", 100);
    uint16_t repetitions = 1;

    if (argv >= 2 && argv <= 3) {
        vec_size = std::stoi(argc[1]);
	std::cout << "Vector size: " << vec_size * sizeof(double) << " B" << "\n";
        
	if (argv == 3) {
	    repetitions = std::stoi(argc[2]);
	    std::cout << "Repetitions: " << repetitions << "\n";
	}
    }

    std::cout << "\n";
    std::cout << "IO performance evaluation code" << "\n";
    std::cout << "Vector size: " << vec_size << "\n";
    std::cout << "\n";

    std::cout << "Allocating memory..." << "\n";
    vec_out.resize(vec_size);
    vec_in.resize(vec_size);

    // fill-in the vector
    std::cout << "Populating the vector with random numbers..." << "\n";
    vec_out.populate();

//    // print the vector
//    vec_out.print();

    for (int n = 0; n < repetitions; ++n) {
	std::cout << " #" << n << "\n";
        std::cout << "Writing vector in ASCII mode..." << "\n";
        results_wa = io_manager.write_vec(FileType::ASCII, vec_out);

        std::cout << "Reading vector in ASCII mode..." << "\n";
        results_ra = io_manager.read_vec(FileType::ASCII, vec_in);

        std::cout << "Writing vector in Binary mode..." << "\n";
        results_wb = io_manager.write_vec(FileType::BINARY, vec_out);

        std::cout << "Reading vector in Binary mode..." << "\n";
        results_rb = io_manager.read_vec(FileType::BINARY, vec_in);

//        std::cout << "\n";
//        vec_in.print();
    
        std::cout << "\n";
        std::cout << "===================================" << "\n";
        std::cout << "            Results" << "\n";
        std::cout << "===================================" << "\n";
        std::cout << "ASCII mode:" << "\n";
        std::cout << " Writing:" << "\n";
        results_wa.print();
        std::cout << "\n";
        std::cout << " Reading:" << "\n";
        results_ra.print();
        std::cout << "-----------------------------------" << "\n";
        std::cout << "Binary mode:" << "\n";
        std::cout << " Writing:" << "\n";
        results_wb.print();
        std::cout << "\n";
        std::cout << " Reading:" << "\n";
        results_rb.print();
        std::cout << "===================================" << "\n";
    }

    return EXIT_SUCCESS;
}
