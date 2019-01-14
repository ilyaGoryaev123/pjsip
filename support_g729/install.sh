#!/bin/bash -e

PJPROJECT_BASE_FOLDER="$1"
PJSIP_VERSION=$2

cd "support_g729"
echo "Installing G729..."

if [ "$PJSIP_VERSION" == "2.8" ]
then
    cp g729.patch $PJPROJECT_BASE_FOLDER
else
    cp g729_old.patch $PJPROJECT_BASE_FOLDER/g729.patch
fi

CURDIR=$(pwd)
cd "$PJPROJECT_BASE_FOLDER"
patch -p0 < g729.patch
rm g729.patch
cd "$CURDIR"
echo "$CURDIR"
cp g729.c "$PJPROJECT_BASE_FOLDER/pjmedia/src/pjmedia-codec/g729.c"
cp g729.h "$PJPROJECT_BASE_FOLDER/pjmedia/include/pjmedia-codec/g729.h"
cp -r g729 "$PJPROJECT_BASE_FOLDER/third_party"
mkdir -p "$PJPROJECT_BASE_FOLDER/third_party/build/g729"
mv "$PJPROJECT_BASE_FOLDER/third_party/g729/Makefile" "$PJPROJECT_BASE_FOLDER/third_party/build/g729"
cd "$PJPROJECT_BASE_FOLDER"
rm -rf aconfigure
autoconf aconfigure.ac > aconfigure
sudo chmod 777 aconfigure
