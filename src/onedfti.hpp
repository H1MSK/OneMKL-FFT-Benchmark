#pragma once
#include "global.hpp"

using OneDftiReal = float;
using OneDftiComplex = complex<float>;

void OneDftiInit();
void OneDftiDeinit();

void OneDftiCompute2D(bool reverse,
                      OneDftiReal* din,
                      OneDftiReal* dout,
                      TimeTest& timer);

inline static OneDftiComplex OneDftiReadComplex(OneDftiReal* data,
                                                int r,
                                                int c) {
    if (r == 0) {
        if (c < (W / 2 + 1)) {
            return OneDftiComplex(data[c * 2], data[c * 2 + 1]);
        } else {
            return OneDftiComplex(data[(W - c) * 2], -data[(W - c) * 2 + 1]);
        }
    } else {
        if (c < (W / 2 + 1)) {
            return OneDftiComplex(data[(r * (W / 2 + 1) + c) * 2],
                                  data[(r * (W / 2 + 1) + c) * 2 + 1]);
        } else {
            return OneDftiComplex(
                data[((H - r) * (W / 2 + 1) + (W - c)) * 2],
                -data[((H - r) * (W / 2 + 1) + (W - c)) * 2 + 1]);
        }
    }
}

inline static OneDftiComplex OneDftiReadComplex(OneDftiComplex* data,
                                                int r,
                                                int c) {
    return data[r * W + c];
}
