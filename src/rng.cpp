/**
 * @file rng.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Wrapper for OneMKL VSL Random Number Generator(RNG)
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#include "rng.hpp"
#include <mkl_vsl.h>
#include <chrono>
#include "global.hpp"

// Assert status, print info and exit on error
#define ASSERT_STATUS(name)                       \
    do {                                          \
        if (status != 0) {                        \
            cout << #name << " returns " << endl; \
            exit(status);                         \
        }                                         \
    } while (0)

void generateRandomNumbers(float* buf, size_t size) {
    VSLStreamStatePtr stream;
    int status = 0;

    // Create a new stream using mt19937 algorithm and seed = current time
    status = vslNewStream(
        &stream, VSL_BRNG_MT19937,
        std::chrono::system_clock::now().time_since_epoch().count());
    ASSERT_STATUS(vslNewStream);

    // Generate uniform distributed random number ranging from -1.0f to 1.0f
    status = vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, size, buf, -1.0f,
                          1.0f);
    ASSERT_STATUS(vsRngUniform);

    // Delete stream to recycle resources
    status = vslDeleteStream(&stream);
    ASSERT_STATUS(vslDeleteStream);
}
