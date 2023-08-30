/**
 * @file soft_fft.hpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Software inplemented fft
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#include <complex>
#include <cstddef>
#include "global.hpp"
#include "time_test.hpp"

#define SOFT_FFT_LENGTH 2048

// Type defines
typedef float SoftFft_Real;
typedef std::complex<SoftFft_Real> SoftFftComplex;

/**
 * @brief Perform initialization
 *
 * calculate unit roots, and reverse bits
 */
void SoftFftInit();

/**
 * @brief Compute 1-dimensional inplace fft/ifft
 *
 * @param reverse true to perform ifft
 * @param data input/output data
 */
void SoftFftComputeInPlace(bool reverse, SoftFftComplex* data);

/**
 * @brief Compute 2-dimensional inplace fft/ifft
 *
 * Perform fft on each row, flip matrix, perform fft on each row again, and flip
 * back
 *
 * @param reverse true to perform ifft
 * @param data input/output data
 * @param timer timer to record transformation time elapse
 */
void SoftFftCompute2DInPlace(bool reverse,
                             SoftFftComplex* data,
                             TimeTest& timer);
