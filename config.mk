CFLAGS = 
CFLAGS += -D DEBUG
CFLAGS += -D LOG

export CFLAGS

export BUILD_DIR = ${CURDIR}/build
export EXEC_NAME = cmank

export GXX = g++