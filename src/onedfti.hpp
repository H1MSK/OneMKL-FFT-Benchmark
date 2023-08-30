/**
 * @file onedfti.hpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief wrappers for OneDfti real-to-complex fft library
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#pragma once
#include "global.hpp"

// Type defines
using OneDftiReal = float;
using OneDftiComplex = complex<float>;

/**
 * @brief Initialize OneDfti handle for transformations
 */
void OneDftiInit();
/**
 * @brief Free OneDfti descriptor handle, recycle resources
 */
void OneDftiDeinit();

/**
 * @brief Perform 2-dimensional fft/ifft with OneDfti
 *
 * @param reverse Perform ifft on true, fft on false
 * @param din input data ptr
 * @param dout output data ptr
 * @param timer timer to record transformation time elapse
 */
void OneDftiCompute2D(bool reverse,
                      OneDftiReal* din,
                      OneDftiReal* dout,
                      TimeTest& timer);

/**
 * @brief Read fft result on row r and column c
 *
 * @note The result of real-to-complex fft is space-optimized using property of
 * fft result conjugation, in order to put complex values in float array.
 *
 * @param data fft result data ptr
 * @param r row number, assuming it is ranged from [0, H)
 * @param c column number, assuming it is ranged from [0, W)
 * @return OneDftiComplex complex data on target position
 */
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

/**
 * @brief Overload function for reading complex data from complex-to-complex fft
 *
 * @note simply returns data[r * W + c]
 *
 * @param data data ptr
 * @param r row number, assuming it is ranged from [0, H)
 * @param c column number, assuming it is ranged from [0, W)
 * @return OneDftiComplex
 */
inline static OneDftiComplex OneDftiReadComplex(OneDftiComplex* data,
                                                int r,
                                                int c) {
    return data[r * W + c];
}
