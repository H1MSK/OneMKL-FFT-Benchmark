set -x

if [ -e build ]; then
    rm -r build/
fi

THREAD_COUNT=`grep -c ^processor /proc/cpuinfo`

mkdir -p build && pushd build

CPU_NAME=`cat /proc/cpuinfo | grep -m1 "model name" | awk '{$1=$2=$3=""; print substr($0,4)}'`
MEM_COUNT=`cat /proc/meminfo | grep MemTotal | awk '{print $2, $3}'`
DIST_NAME=`cat /etc/os-release | grep PRETTY_NAME | sed -En 's/^.+\"(.+)\".*$/\1/p'`
ONEAPI_VERSION=`ls -al /opt/intel/oneapi/compiler/  | grep latest | sed -En 's/.+latest -> (.+)$/\1/p'`
echo -e "CPU:\t$CPU_NAME x $THREAD_COUNT\nMEM:\t$MEM_COUNT\t\nDIST:\t$DIST_NAME\nONEAPI:\t$ONEAPI_VERSION" >>../result.txt

for MKL_THREADING in sequential intel_thread gnu_thread pgi_thread tbb_thread; do
    for MKL_MPI in intelmpi openmpi mpich mpich2; do
        for WITH_OFAST in OFF ON; do
            echo -e "\n\nMKL_THREADING=$MKL_THREADING MKL_MPI=$MKL_MPI WITH_OFAST=$WITH_OFAST:" >>../result.txt
            rm -r *
            cmake .. -DMKL_THREADING=$MKL_THREADING -DMKL_MPI=$MKL_MPI -DWITH_OFAST=$WITH_OFAST
            if (( $? != 0 )); then
                echo "Configure error" >>../result.txt
                continue
            fi

            make -j$THREAD_COUNT
            if (( $? != 0 )); then
                echo "Compile error" >>../result.txt
                continue
            fi

            ./main >>../result.txt
        done
    done
done
popd

rm -r build
