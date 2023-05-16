//
// Created by Maksim Masterov on 16/05/2023.
//

#ifndef IO_BENCH_IOMANAGER_H
#define IO_BENCH_IOMANAGER_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

#include "Vector.h"

enum FileType {
    BINARY,
    ASCII
};

class Results {
    double elp_time;    // [ms]
    size_t file_size;   // [B]
    double speed;       // [MB/s]

public:
    Results() : elp_time(0.0), file_size(0), speed(0.0) {}
    ~Results() = default;

    inline void fill_in(double _elp_time, size_t _file_size) {
        elp_time = _elp_time;
        file_size = _file_size;
        speed = (double) file_size / 1024.0 / 1024.0 / elp_time * 1e3;
    }

    inline void print() const {
        std::cout << "  Elapsed time: " << elp_time << " ms" << "\n";
        std::cout << "  File size:    " << file_size << " B" << "\n";
        std::cout << "  Speed:        " << speed << " MB/s" << "\n";
    }
};

class IOManager {
public:
    explicit IOManager(const std::string& _file_base_name) {
        file_base_name = _file_base_name;
        file_ext_ascii = "dat";
        file_ext_binary = "bin";
    }
    ~IOManager() = default;

    inline Results write_vec(const int type, const Vector<double> &vec) {

        std::string filename;
        Results results;
        int err_code = EXIT_SUCCESS;

        filename = assemble_filename(type);

        // Delete file first
        delete_file(filename);

        auto begin = std::chrono::high_resolution_clock::now();

        if (type == FileType::ASCII) {
            err_code = write_ascii(filename, vec);
        }
        else if (type == FileType::BINARY) {
            err_code = write_binary(filename, vec);
        } else {
            std::cerr << "Error! Unknown file type: " << type << std::endl;
        }

        if (err_code != EXIT_SUCCESS) {
            std::cerr << "Error! Can't open the file: " << filename << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elp_time = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()
                                / 1e+6;

        results.fill_in(elp_time, get_file_size(filename));

        return results;
    }

    inline Results read_vec(const int type, Vector<double> &vec) {

        std::string filename;
        Results results;
        int err_code = EXIT_SUCCESS;

        filename = assemble_filename(type);

        auto begin = std::chrono::high_resolution_clock::now();

        if (type == FileType::ASCII) {
            err_code = read_ascii(filename, vec);
        }
        else if (type == FileType::BINARY) {
            err_code = read_binary(filename, vec);
        } else {
            std::cerr << "Error! Unknown file type: " << type << std::endl;
        }

        if (err_code != EXIT_SUCCESS) {
            std::cerr << "Error! Can't open the file: " << filename << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elp_time = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count()
                                / 1e+6;

        results.fill_in(elp_time, get_file_size(filename));

        return results;
    }

private:

    inline std::string assemble_filename(const int type) {
        std::string filename = "None";
        if (type == FileType::ASCII) filename = file_base_name + '.' + file_ext_ascii;
        else if (type == FileType::BINARY) filename = file_base_name + '.' + file_ext_binary;
        else {
            std::cerr << "Error! Unknown file type: " << type << std::endl;
            exit(1);
        }
        return filename;
    }

    static inline size_t get_file_size(std::string& filename)
    {
        struct stat stat_buf{};
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }

    static inline void delete_file(const std::string& filename) {

        std::cout << "Checking if file exists: " << filename << "...\n";
        try {
            if (std::filesystem::remove(filename)) {
                std::cout << "File exists, deleting it...\n";
            }
            else {
                std::cout << "File doesn't exist: " << filename << "...\n";
            }
        }
        catch(const std::filesystem::filesystem_error& err) {
            std::cout << "File system error: " << err.what() << '\n';
        }
    }

    static int write_ascii(const std::string& filename, const Vector<double> &vec) {

        std::ofstream out;
        int precision = 5;

        out.open(filename, std::ios::out);

        // Set the precission
        out.setf(std::ios::scientific);
        out.precision(precision);

        if (out.is_open()) {
            for (auto v: vec.getContainer()) {
                out << v << "\n";
            }
            out.close();
        } else {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    static int write_binary(const std::string& filename, const Vector<double> &vec) {

        std::ofstream out;
        out.open(filename, std::ios::out | std::ios::binary);

        if (out.is_open()) {
            for (auto v: vec.getContainer()) {
                out.write((char*) &v, sizeof(double ));
            }
            out.close();
        } else {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    static int read_ascii(const std::string& filename, Vector<double> &vec) {

        std::ifstream in;

        in.open(filename, std::ios::in);

        if (in.is_open()) {
            for (auto & v: vec.getContainer()) {
                in >> v;
            }
            in.close();
        } else {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    static int read_binary(const std::string& filename, Vector<double> &vec) {

        std::ifstream in;
        in.open(filename, std::ios::in | std::ios::binary);

        if (in.is_open()) {
            for (auto &v: vec.getContainer()) {
                in.read((char*) &v, sizeof(double ));
            }
            in.close();
        } else {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

private:
    std::string file_base_name;
    std::string file_ext_ascii;
    std::string file_ext_binary;
};


#endif //IO_BENCH_IOMANAGER_H
