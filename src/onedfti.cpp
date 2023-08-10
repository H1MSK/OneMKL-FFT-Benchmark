#include "onedfti.hpp"
#include <chrono>
#include <oneapi/mkl.hpp>
#include "global.hpp"

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
    status = DftiCreateDescriptor(&handle, DFTI_SINGLE, DFTI_REAL, (MKL_LONG)2,
                                  dim_sizes);
    ASSERT_STATUS(DftiCreateDescriptor);

    DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);

    status = DftiCommitDescriptor(handle);
    ASSERT_STATUS(DftiCommitDescriptor);
}

void OneDftiDeinit() {
    MKL_LONG status;

    status = DftiFreeDescriptor(&handle);
    ASSERT_STATUS(DftiFreeDescriptor);
}

void OneDftiCompute2D(bool reverse,
                      OneDftiReal* din,
                      OneDftiReal* dout,
                      TimeTest& timer) {
    MKL_LONG status;
    // DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    // DftiSetValue(handle, DFTI_FORWARD_DOMAIN, DFTI_REAL);
    // DftiSetValue(handle, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
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
