//
// Created by Maksim Masterov on 16/05/2023.
//

#ifndef IO_BENCH_VECTOR_H
#define IO_BENCH_VECTOR_H

#include <iostream>
#include <vector>
#include <random>
#include <type_traits>


template <typename T>
class Vector {
private:
    using uniform_distribution =
            typename std::conditional<
                    std::is_floating_point<T>::value,
                    std::uniform_real_distribution<T>,
                    typename std::conditional<
                            std::is_integral<T>::value,
                            std::uniform_int_distribution<T>,
                            void
                    >::type
            >::type;

public:
    Vector() = default;
    ~Vector() = default;

    /**
     * @brief Populate vector with random values uniformly distributed in the given range.
     * @param lower Lower bound
     * @param upper Upper bound
     */
    inline void populate(const T lower = (T)0, const T upper = (T)10) {
        std::random_device rd;
        std::mt19937 gen(rd());
        uniform_distribution dis(lower, upper);

        for(auto & v : vec )
            v = dis(gen);
    }

    inline void print() {
        for(auto v : vec )
            std::cout << v << "\n";
    }

    inline void resize(const size_t size) {
        vec.resize(size, (T)0);
    }

    inline void clear() {
        vec.clear();
    }

    inline void zero() {
        for (auto & v : vec)
            v = (T)0;
    }

    inline std::vector<T>& getContainer() {
        return static_cast<std::vector<double> &>(vec);
    }

    inline std::vector<T>& getContainer() const {
        return const_cast<std::vector<double> &>(vec);
    }

private:
    std::vector<T> vec;
};


#endif //IO_BENCH_VECTOR_H
