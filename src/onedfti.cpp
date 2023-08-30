/**
 * @file onedfti.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief wrappers for OneDfti real-to-complex fft library
 * @version 1.0
 * @date 2023-08-30
 *
 * @copyright © 2023 H1MSK
 *
 */
#include "onedfti.hpp"
#include <chrono>
#include <oneapi/mkl.hpp>
#include "global.hpp"

// Assert status, print info and exit on error
#define ASSERT_STATUS(name)                                     \
    do {                                                        \
        if (status && !DftiErrorClass(status, DFTI_NO_ERROR)) { \
            cout << #name << " returns " << status << ":"       \
                 << DftiErrorMessage(status) << endl;           \
            exit(status);                                       \
        }                                                       \
    } while (0)

DFTI_DESCRIPTOR_HANDLE handle;
MKL_LONG dim_sizes[2] = {W, H};

void OneDftiInit() {
    MKL_LONG status;
    // Create a descriptor for real-to-complex 2-dimensional fft with single
    // presicion and dim_sizes
    status = DftiCreateDescriptor(&handle, DFTI_SINGLE, DFTI_REAL, (MKL_LONG)2,
                                  dim_sizes);
    ASSERT_STATUS(DftiCreateDescriptor);

    // To satisfy r2c fft requirement
    DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);

    // Commit descriptor to hardware
    status = DftiCommitDescriptor(handle);
    ASSERT_STATUS(DftiCommitDescriptor);
}

void OneDftiDeinit() {
    MKL_LONG status;

    // Free descriptor and release resources occupied
    status = DftiFreeDescriptor(&handle);
    ASSERT_STATUS(DftiFreeDescriptor);
}

void OneDftiCompute2D(bool reverse,
                      OneDftiReal* din,
                      OneDftiReal* dout,
                      TimeTest& timer) {
    MKL_LONG status;

    // Call forward or backward fft on condition, and record time elapsed
    if (reverse) {
        timer.start();
        status = DftiComputeBackward(handle, din, dout);
        timer.stop();
        ASSERT_STATUS(DftiComputeBackward);
    } else {
        timer.start();
        status = DftiComputeForward(handle, din, dout);
        timer.stop();
        ASSERT_STATUS(DftiComputeForward);
    }
}
