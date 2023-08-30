/**
 * @file main.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Main file, include code for global variables, testbench initialization
 * and finalization
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#include "global.hpp"
#include "onedfti.hpp"
#include "rng.hpp"
#include "soft_fft.hpp"
#include "time_test.hpp"

// Buffer for RNG generation
float buffer[N];
// For software fft inplace transform
SoftFftComplex buf_soft[(W + 1) * H];

// For real-to-complex transform, the buffer should be larger to store complex
// data
OneDftiReal buf_one_in[(W + 2) * (H * 2)];
OneDftiReal buf_one_fft[(W + 2) * (H * 2)];

// Timer class for both software and dfti transformations
TimeTest soft_timer_f, soft_timer_b;
TimeTest dfti_timer_f, dfti_timer_b;

// Statistic classes
Stat<float> error_fw, error_bw;

/**
 * @brief Copy rngs from buffer to buf_soft and buf_one_in
 */
void prepareBuffers() {
    for (int i = 0, ptr = 0; i < W; ++i) {
        for (int j = 0; j < H; ++j) {
            float t = buffer[ptr];
            buf_soft[i * (W + 1) + j] = t;
            buf_one_in[i * W + j] = t;
            ptr++;
        }
    }
}

/**
 * @brief Get max distance as difference between soft and dfti calculation
 *
 * @param reverse whether to compare fft or ifft result
 * @return float max distance
 */
float checkDistance(bool reverse) {
    float max_distance = 0;
    for (int i = 0; i < W; ++i) {
        for (int j = 0; j < H; ++j) {
            auto distance = (reverse ? buf_one_in[i * W + j]
                                     : OneDftiReadComplex(buf_one_fft, i, j)) -
                            OneDftiComplex(buf_soft[i * (W + 1) + j]);
            auto this_distance = abs(distance) / (reverse ? W : 1);
            (reverse ? error_bw : error_fw).add(this_distance);
            if (this_distance > max_distance)
                max_distance = this_distance;
        }
        break;
    }
    return max_distance;
}

/**
 * @brief Compare ratios of max/min/avg between two @class Stat, return a
 * human-readable string
 *
 * @param s1 Stat as a reference
 * @param s2 Stat that will be compared
 * @return std::string human-readable string representing ratios of Stats
 */
std::string compare(const Stat<std::chrono::nanoseconds>& s1,
                    const Stat<std::chrono::nanoseconds>& s2) {
    return std::string("max=") +
           std::to_string((float)s2.max().count() / s1.max().count()) +
           "x, min=" +
           std::to_string((float)s2.min().count() / s1.min().count()) +
           "x, avg=" +
           std::to_string((float)s2.avg().count() / s1.avg().count()) + 'x';
}

int main() {
    // Initializations
    SoftFftInit();
    OneDftiInit();

    soft_timer_f.reset();
    soft_timer_b.reset();
    dfti_timer_f.reset();
    dfti_timer_b.reset();

    error_fw.reset();
    error_bw.reset();

    // Test for 1024 times
    for (int iter = 0; iter < 1024; ++iter) {
        // Generate random numbers and prepare buffers for soft and dfti ffts
        generateRandomNumbers(buffer, N);
        prepareBuffers();

        // Perform software fft, and record result with soft_timer_f
        SoftFftCompute2DInPlace(false, buf_soft, soft_timer_f);

        // Perform dfti fft, and record result with dfti_timer_f
        OneDftiCompute2D(false, buf_one_in, buf_one_fft, dfti_timer_f);

        // Get max forward distance
        float fw_dis = checkDistance(false);

        // Perform software ifft, and record result with soft_timer_b
        SoftFftCompute2DInPlace(true, buf_soft, soft_timer_b);

        // Perform dfti ifft, and record result with dfti_timer_b
        OneDftiCompute2D(true, buf_one_fft, buf_one_in, dfti_timer_b);

        // Get max distance for inverse fft
        float bw_dis = checkDistance(true);

        // Print some info when running
        cerr << "iter #" << iter << ": fw error=" << fw_dis
             << ", bw error=" << bw_dis << '\n'
             << flush;
    }

    // Stat formatters
    auto fmt_nano = [](std::chrono::nanoseconds x) {
        return std::to_string(x.count()) + "ns";
    };

    cout << "\nTime:\n\tForward:\n"

         << "\t\tOneAPI   " << dfti_timer_f.stats().to_string(fmt_nano) << '\n'

         << "\t\tSoftware " << soft_timer_f.stats().to_string(fmt_nano) << '\n'
         << "\t\tBoosted  "
         << compare(dfti_timer_f.stats(), soft_timer_f.stats()) << '\n'

         << "\tBackward:\n"

         << "\t\tOneAPI   " << dfti_timer_b.stats().to_string(fmt_nano) << '\n'
         << "\t\tSoftware " << soft_timer_b.stats().to_string(fmt_nano) << '\n'
         << "\t\tBoosted  "
         << compare(dfti_timer_b.stats(), soft_timer_b.stats()) << '\n'

         << "Error:\n"
         << "\tForward  "
         << error_fw.to_string([](float x) { return std::to_string(x); })
         << '\n'
         << "\tBackward "
         << error_bw.to_string([](float x) { return std::to_string(x); })
         << '\n'
         << flush;

    return 0;
}
