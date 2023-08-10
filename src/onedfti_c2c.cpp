#include "onedfti.hpp"
#include <chrono>
#include <oneapi/mkl.hpp>
#include "global.hpp"

DFTI_DESCRIPTOR_HANDLE handle;
MKL_LONG dim_sizes[2] = {W, H};

void OneDftiInit() {
    MKL_LONG status;
    status =
        DftiCreateDescriptor(&handle, DFTI_SINGLE, DFTI_COMPLEX, 2, dim_sizes);
    ASSERT_STATUS(DftiCreateDescriptor);

    status = DftiCommitDescriptor(handle);
    ASSERT_STATUS(DftiCommitDescriptor);
}

void OneDftiDeinit() {
    MKL_LONG status;

    status = DftiFreeDescriptor(&handle);
    ASSERT_STATUS(DftiFreeDescriptor);
}

void OneDftiCompute2DInPlace(bool reverse,
                             OneDftiComplex* data,
                             TimeTest& timer) {
    MKL_LONG status;
    if (reverse) {
        timer.start();
        status = DftiComputeBackward(handle, data);
        timer.stop();
        ASSERT_STATUS(DftiComputeBackward);
    } else {
        timer.start();
        status = DftiComputeForward(handle, data);
        timer.stop();
        ASSERT_STATUS(DftiComputeForward);
    }
}
