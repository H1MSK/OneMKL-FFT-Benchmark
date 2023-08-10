#pragma once
#include "global.hpp"

using OneDftiComplex = complex<float>;

void OneDftiInit();
void OneDftiDeinit();

void OneDftiCompute2DInPlace(bool reverse,
                             OneDftiComplex* data,
                             TimeTest& timer);
