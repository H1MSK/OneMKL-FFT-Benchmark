#include "soft_fft.hpp"
#include <cassert>
#include <cmath>

int bit_rev[W];
SoftFftComplex omega[W];
SoftFftComplex rev_omega[W];

void SoftFftInit() {
    // Make sure n is power of two
    assert(W > 0 &&
           (W & (W - 1)) == 0);

    SoftFft_Real PI = acos(-1);

    int N, log2N;

    for (N = 1, log2N = 0; N < W; N <<= 1, log2N++)
        ;

    for (int j = 0; j < W; ++j) {
        omega[j] = SoftFftComplex(cos(2 * PI * j / N), -sin(2 * PI * j / N));
        rev_omega[j] = conj(omega[j]);
    }

    for (int i = 0; i < N; ++i)
        bit_rev[i] = (bit_rev[i >> 1] >> 1) | ((i & 1) << (log2N - 1));
}

const static SoftFft_Real PI = acos(-1);
void SoftFftComputeInPlace(bool reverse, SoftFftComplex* data) {
    // Destroy input, swap items in advance
    for (int i = 0; i < W; ++i) {
        if (i < bit_rev[i]) {
            SoftFftComplex Temp = data[i];
            data[i] = data[bit_rev[i]];
            data[bit_rev[i]] = Temp;
        }
    }

    // Perform butterfly operation
    SoftFftComplex* O = reverse ? rev_omega : omega;
    for (int l = 2; l <= W; l <<= 1) {
        int m = l >> 1;
        for (int k = 0; k < W; k += l)
            for (int i = 0; i < m; ++i) {
                SoftFftComplex t = data[k + m + i] * O[W / l * i];
                data[k + m + i] = data[k + i] - t;
                data[k + i] += t;
            }
    }

    // if (reverse) {
    //     for (int i = 0; i < W; ++i) {
    //         data[i] /= W;
    //     }
    // }
}

static void SoftFftFlipMatrix(SoftFftComplex* data) {
#pragma omp parallel for
    for (int i = 1; i < W; ++i) {
        for (int j = 0; j < i; ++j) {
            SoftFftComplex temp = data[i * (W + 1) + j];
            data[i * (W + 1) + j] = data[j * (W + 1)+ i];
            data[j * (W + 1) + i] = temp;
        }
    }
}

void SoftFftCompute2DInPlace(bool reverse, SoftFftComplex* data, TimeTest& timer) {
// do fft for each row
    timer.start();
#pragma omp parallel for
    for (int i = 0; i < W; ++i) {
        SoftFftComputeInPlace(reverse, data + i * (W + 1));
    }

    // make a flip
    SoftFftFlipMatrix(data);

// do fft for each row of flipped matrix, i.e. each column of original matrix
#pragma omp parallel for
    for (int i = 0; i < W; ++i) {
        SoftFftComputeInPlace(reverse, data + i * (W + 1));
    }

    // flip back
    SoftFftFlipMatrix(data);
    timer.stop();
}
