#!/bin/sh

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
    OPENSSL_URL="https://github.com/x2on/OpenSSL-for-iPhone/archive/1.0.2i.tar.gz"
    #OPENSSL_URL="https://raw.githubusercontent.com/x2on/OpenSSL-for-iPhone/master/build-libssl.sh"
    OPENSSL_SH="build-libssl.sh"


    if [ ! -f "${OPENSSL_DIR}/lib/libssl.a" ]; then
        download ${OPENSSL_URL} ${OPENSSL_DIR}
        pushd . > /dev/null
        cd ${OPENSSL_DIR}
        /bin/sh ${OPENSSL_SH}
        mv include include2
        mkdir -p include
        mv include2 include/openssl
        popd > /dev/null
    else
        echo "Using OpenSSL..."
    fi

    OPENSSL_ENABLED=1
}

# openh264
OPENH264_DIR="${BUILD_DIR}/openh264"
OPENH264_ENABLED=
function openh264() {
    if [ ! -f "${OPENH264_DIR}/lib/libopenh264.a" ] || [ ! -d "${OPENH264_DIR}/include/wels/" ]; then
        "${__DIR__}/openh264.sh" "${OPENH264_DIR}"
    else
        echo "Using OpenH264..."
    fi

    OPENH264_ENABLED=1
}

G729_ENABLED=
function enable_g729() {
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
    if [[ OPENH264_ENABLED ]]; then
        ARGS+=("--with-openh264" "${OPENH264_DIR}")
    fi
    if [[ OPENSSL_ENABLED ]]; then
        ARGS+=("--with-openssl" "${OPENSSL_DIR}")
    fi
    if [[ OPUS_ENABLED ]]; then
        ARGS+=("--with-opus" "${OPUS_DIR}/dependencies")
    fi
    if [[ G729_ENABLED ]]; then
        ARGS+=("--enable-g729-codec")
    fi
    echo "${ARGS[@]}"

    "${__DIR__}/pjsip.sh" "${PJSIP_DIR}" "${ARGS[@]}"
}

openssl
openh264
opus
enable_g729
pjsip
