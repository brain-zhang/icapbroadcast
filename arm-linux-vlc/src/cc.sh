#!/bin/sh
# for arm

CC=/opt/FriendlyARM/tools_chain/bin/arm-none-linux-gnueabi-gcc \
CXX=/opt/FriendlyARM/tools_chain/bin/arm-none-linux-gnueabi-g++  \
ARCH=arm \
#--host=arm-none-linux-gnueabi \
build_alias=i486-linux-gnu \
CFLAGS='-g -O2' \
LDFLAGS=-Wl,--as-needed \
CPPFLAGS= \
CXXFLAGS='-g -O2' 
