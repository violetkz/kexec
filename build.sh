#!/bin/sh

BUILD_DIR=./build
BUILD_TYPE=Release

if [ $# -gt 0 ];then 
    case "$1" in 
        "-d" | "debug") 
            BUILD_TYPE=Debug
            ;;
        "-h" | "--help" | "help")
            echo "$ $0  [[-d|debug][-h|--help|help]]"
            ;;
        "-c" | "--clean" | "clean")
            rm -rf $BUILD_DIR kexeclib
            exit 0
            ;;
        *)
            ;;
    esac
fi

if which cmake 2>&1 >/dev/null; then
    [ -d $BUILD_DIR ] || mkdir $BUILD_DIR || exit 1
    cd $BUILD_DIR && cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..  && make install
else 
    echo "Plese install cmake for build"
    exit 1
fi
