list(APPEND SOURCE_FILE
    main.cpp
    onedfti.cpp
    onedfti.hpp
    soft_fft.cpp
    soft_fft.hpp
    rng.cpp
    rng.hpp
    stat.hpp
    time_test.hpp)


set(TARGET_NAME main)


set(CMAKE_MODULE_PATH "/opt/intel/oneapi/mkl/latest/lib/cmake/mkl;${CMAKE_MODULE_PATH}")

# This is a Windows-specific flag that enables exception handling in host code
if(WIN32)
    set(WIN_FLAG "/EHsc")
endif()

# 
# SECTION 1
# This section defines rules to create a cpu-gpu make target
# This can safely be removed if your project is only targetting FPGAs
#

# set(MKL_MPI "openmpi")
option(ENABLE_BLACS "ENABLE_BLACS" ON)
option(ENABLE_CDFT "ENABLE_CDFT" OFF)
option(ENABLE_OMP_OFFLOAD "ENABLE_OMP_OFFLOAD" ON)
find_package(MKL)

set(COMPILE_FLAGS "-fsycl -fopenmp ${WIN_FLAG} -I/opt/intel/oneapi/mkl/latest/include")
set(LINK_FLAGS "-fsycl -fopenmp -rpath /opt/intel/oneapi/compiler/latest/linux/compiler/lib/intel64_lin/ -L/opt/intel/oneapi/mkl/latest/lib/intel64 -L/opt/intel/oneapi/compiler/latest/linux/lib")

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message(STATUS "Build type: Debug")
    set(COMPILE_FLAGS "-g ${COMPILE_FLAGS}")
    set(LINK_FLAGS "-g ${LINK_FLAGS}")
else()
    message(STATUS "Build type: Release")
    if (WITH_OFAST)
        message(STATUS "Compile with -Ofast")
        set(COMPILE_FLAGS "-Ofast ${COMPILE_FLAGS}")
        set(LINK_FLAGS "-Ofast ${LINK_FLAGS}")
    else()
    message(STATUS "Compile with -O2")
    set(COMPILE_FLAGS "-O2 ${COMPILE_FLAGS}")
        set(LINK_FLAGS "-O2 ${LINK_FLAGS}")
    endif()
endif()

# To compile in a single command:
#    icpx -fsycl <file>.cpp -o <file>.fpga_emu
# CMake executes:
#    [compile] icpx -fsycl -o <file>.cpp.o -c <file>.cpp
#    [link]    icpx -fsycl <file>.cpp.o -o <file>.fpga_emu

add_executable(${TARGET_NAME} ${SOURCE_FILE} ${FFTW3_SRCs})
set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
target_link_libraries(${TARGET_NAME} PUBLIC
    MKL::MKL_DPCPP
    ${CMAKE_DL_LIBS}
)
# 
# End of SECTION 1
#
