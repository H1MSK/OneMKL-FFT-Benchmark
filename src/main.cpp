#include <mkl.h>
#include "global.hpp"
#include "onedfti.hpp"
#include "rng.hpp"
#include "soft_fft.hpp"
#include "time_test.hpp"

float buffer[N];
SoftFftComplex buf_soft[(W + 1) * H];

// For real-to-complex inplace transform, the buffer should be larger to store
// complex data
OneDftiReal buf_one_in[(W + 2) * (H * 2)];
OneDftiReal buf_one_fft[(W + 2) * (H * 2)];

TimeTest soft_timer_f, soft_timer_b;
TimeTest dfti_timer_f, dfti_timer_b;

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

float checkDistance(bool reverse) {
    float max_distance = 0;
    for (int i = 0; i < W; ++i) {
        for (int j = 0; j < H; ++j) {
            auto distance = (reverse ? buf_one_in[i * W + j]
                                     : OneDftiReadComplex(buf_one_fft, i, j)) -
                            OneDftiComplex(buf_soft[i * (W + 1) + j]);
            auto this_distance = abs(distance);
            if (this_distance > max_distance)
                max_distance = this_distance;
        }
        break;
    }
    return max_distance;
}

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
    SoftFftInit();
    OneDftiInit();

    soft_timer_f.reset();
    soft_timer_b.reset();
    dfti_timer_f.reset();
    dfti_timer_b.reset();

    Stat<float> error_fw, error_bw;

    error_fw.reset();
    error_bw.reset();

    for (int iter = 0; iter < 256; ++iter) {
        generateRandomNumbers(buffer, N);
        prepareBuffers();

        SoftFftCompute2DInPlace(false, buf_soft, soft_timer_f);
        OneDftiCompute2D(false, buf_one_in, buf_one_fft, dfti_timer_f);

        float fw_dis = checkDistance(false);
        error_fw.add(fw_dis);

        SoftFftCompute2DInPlace(true, buf_soft, soft_timer_b);
        OneDftiCompute2D(true, buf_one_fft, buf_one_in, dfti_timer_b);

        float bw_dis = checkDistance(true) / W;
        error_bw.add(bw_dis);

        cerr << "iter #" << iter << ": fw error=" << fw_dis
             << ", bw error=" << bw_dis << '\n'
             << flush;
    }

    auto fmt_float = [](float x) { return std::to_string(x); };
    auto fmt_nano = [](std::chrono::nanoseconds x) {
        return std::to_string(x.count()) + "ns";
    };

    cout << "\nTime:\n\tForward:\n"

         << "\t\tOneAPI   " << dfti_timer_f.stats().to_string(fmt_nano) << '\n'

         << "\t\tSoftware " << soft_timer_f.stats().to_string(fmt_nano) << '\n'
         << "\t\tBoosted  " << compare(dfti_timer_f.stats(), soft_timer_f.stats())
         << '\n'

         << "\tBackward:\n"

         << "\t\tOneAPI   " << dfti_timer_b.stats().to_string(fmt_nano) << '\n'
         << "\t\tSoftware " << soft_timer_b.stats().to_string(fmt_nano) << '\n'
         << "\t\tBoosted  " << compare(dfti_timer_b.stats(), soft_timer_b.stats())
         << '\n'

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
