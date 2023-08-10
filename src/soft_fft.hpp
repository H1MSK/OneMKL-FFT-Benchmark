#pragma once
#include <complex>
#include <cstddef>
#include "global.hpp"
#include "time_test.hpp"

#define SOFT_FFT_LENGTH 2048

typedef float SoftFft_Real;
typedef std::complex<SoftFft_Real> SoftFftComplex;

void SoftFftInit();

void SoftFftComputeInPlace(bool reverse, SoftFftComplex* data);

void SoftFftCompute2DInPlace(bool reverse,
                             SoftFftComplex* data,
                             TimeTest& timer);
