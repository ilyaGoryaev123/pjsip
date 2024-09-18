#!/bin/sh

set -e

# environment variables
export OPENSSL_VERSION="1.1.1c" # specify the openssl version to use
export PJSIP_VERSION="2.14.1"
export OPUS_VERSION="1.3.1"
export G729_VERSION="1.1.1"
export OPENH264_VERSION="2.2.0"
export MACOS_MIN_SDK_VERSION="10.12"
export IOS_MIN_SDK_VERSION="9.0"

# see http://stackoverflow.com/a/3915420/318790
function realpath { echo $(cd $(dirname "$1"); pwd)/$(basename "$1"); }
__FILE__=`realpath "$0"`
__DIR__=`dirname "${__FILE__}"`

BUILD_DIR="${__DIR__}/build"
if [ ! -d ${BUILD_DIR} ]; then
    mkdir ${BUILD_DIR}
fi

# download
function download() {
    "${__DIR__}/download.sh" "$1" "$2" #--no-cache
}

# openssl
OPENSSL_DIR="${BUILD_DIR}/openssl"
OPENSSL_ENABLED=
function openssl() {
    if [ ! -d "${OPENSSL_DIR}/lib/iOS" ] || [ ! -d "${OPENSSL_DIR}/lib/macOS" ]; then
        if [ ! -d "${OPENSSL_DIR}" ]; then
            mkdir -p "${OPENSSL_DIR}"
        fi
        "${__DIR__}/openssl/openssl.sh" "--version=${OPENSSL_VERSION}" "--reporoot=${OPENSSL_DIR}" "--macos-min-sdk=${MACOS_MIN_SDK_VERSION}" "--ios-min-sdk=${IOS_MIN_SDK_VERSION}"
    else
        echo "Using OpenSSL..."
    fi

    OPENSSL_ENABLED=1
}

# bcg729
G729_DIR="${BUILD_DIR}/bcg729"
G729_ENABLED=
function bcg729() {
    if [ ! -f "${G729_DIR}/lib/libbcg729.a" ] || [ ! -d "${G729_DIR}/include/bcg729/" ]; then
        "${__DIR__}/bcg729.sh" "${G729_DIR}"
    else
        echo "Using BCG729..."
    fi

    G729_ENABLED=1
}

# opus
OPUS_DIR="${BUILD_DIR}/opus"
OPUS_ENABLED=
function opus() {
    if [ ! -f "${OPUS_DIR}/dependencies/lib/libopus.a" ] || [ ! -d "${OPUS_DIR}/dependencies/include/opus/" ]; then
        "${__DIR__}/opus.sh" "${OPUS_DIR}"
    else
        echo "Using OPUS..."
    fi

    OPUS_ENABLED=1
}

PJSIP_DIR="${BUILD_DIR}/pjproject"
function pjsip() {
    ARGS=
    if [[ OPENSSL_ENABLED ]]; then
        ARGS+=("--with-openssl" "${OPENSSL_DIR}")
    fi
    if [[ OPUS_ENABLED ]]; then
        ARGS+=("--with-opus" "${OPUS_DIR}/dependencies")
    fi
    if [[ G729_ENABLED ]]; then
        ARGS+=("--with-g729" "${G729_DIR}")
    fi
    echo "${ARGS[@]}"

    "${__DIR__}/pjsip.sh" "${PJSIP_DIR}" "${ARGS[@]}"
}

openssl
opus
bcg729
pjsip
