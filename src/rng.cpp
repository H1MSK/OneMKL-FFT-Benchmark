#include "rng.hpp"
#include <mkl_vsl.h>
#include <chrono>
#include "global.hpp"

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

    status = vslNewStream(
        &stream, VSL_BRNG_MT19937,
        std::chrono::system_clock::now().time_since_epoch().count());
    ASSERT_STATUS(vslNewStream);

    status = vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, size, buf, -1.0f,
                          1.0f);
    ASSERT_STATUS(vsRngUniform);

    status = vslDeleteStream(&stream);
    ASSERT_STATUS(vslDeleteStream);
}
