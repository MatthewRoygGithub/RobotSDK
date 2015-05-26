isEmpty(CUDA_SOURCES){
    message(Please add .cu files to CUDA_SOURCES variable in .pro file)
}
else{
    CUDA_DIR = /usr/local/cuda
    CUDA_ARCH = sm_35

    INCLUDEPATH  += $$CUDA_DIR/include
    LIBS += -L$$CUDA_DIR/lib64 -lcudart
    LIBS += -L$$CUDA_DIR/lib64 -lcuda

    CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')
    NVCCFLAGS = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v

    cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -O3 -arch=$$CUDA_ARCH -c $$NVCCFLAGS \
                    $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT} \
                    2>&1 | sed -r \"s/\\(([0-9]+)\\)/:\\1/g\" 1>&2
    cuda.dependency_type = TYPE_C
    cuda.depend_command = $$CUDA_DIR/bin/nvcc -O3 -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}
    cuda.input = CUDA_SOURCES
    cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
    QMAKE_EXTRA_COMPILERS += cuda

    SOURCES += $$CUDA_SOURCES
    SOURCES -= $$CUDA_SOURCES
}
