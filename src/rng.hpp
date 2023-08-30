/**
 * @file rng.hpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Wrapper for OneMKL VSL Random Number Generator(RNG)
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#pragma once
#include <cstddef>

/**
 * @brief Generate uniform distributed random numbers in (-1.0f, 1.0f) using
 * OneMKL VSL library
 *
 * @param buf buffer to store generated numbers
 * @param size count of numbers to be generated
 */
void generateRandomNumbers(float* buf, size_t size);
