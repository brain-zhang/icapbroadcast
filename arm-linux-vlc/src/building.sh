#!/bin/sh

# for arm
CC=/opt/FriendlyARM/tools_chain/bin/arm-none-linux-gnueabi-gcc /
CXX=/opt/FriendlyARM/tools_chain/bin/arm-none-linux-gnueabi-g++  /
ARCH=arm /
    --host=arm-none-linux-gnueabi /

build_alias=i486-linux-gnu /
CFLAGS='-g -O2' /
LDFLAGS=-Wl,--as-needed /
CPPFLAGS= /
CXXFLAGS='-g -O2' /
./configure  /
    --build=i486-linux-gnu /
    --enable-run-as-root /
    --prefix=/tmp/vlc /
    --with-binary-version=2ubuntu1 /
    /
    --enable-debug /
    --disable-hal /
    --disable-mtp /
    --disable-dbus /
    --disable-dbus-control /
    --disable-telepathy /
    --disable-mmx /
    --disable-sse /
    --disable-lua /
    --disable-notify /
    --disable-audioscrobbler /
    --disable-taglib /
    --disable-live555 /
    --disable-dc1394 /
    --disable-dv /
    --disable-dvdnav /
    --disable-smb /
    --disable-dvbpsi /
    --disable-v4l2 /
    --disable-libcdio /
    --disable-libvcdinfo /
    --disable-libcddb /
    --disable-cdda /
    --disable-vcd /
    --disable-dvb /
    --disable-ogg /
    --disable-mkv /
    --disable-mod /
    --disable-mpc /
    --disable-gme /
    --disable-mad /
    --disable-avcodec /
    --disable-avformat /
    --disable-swscale /
    --disable-postproc /
    --disable-libsysfs /
    --disable-libtar /
    --disable-a52 /
    --disable-dca /
    --disable-zvbi /
    --disable-x11 /
    --disable-xvideo /
    --disable-glx /
    --disable-xinerama /
    --disable-xcb /
    --disable-opengl /
    --disable-sdl /
    --disable-sdl-image /
    --disable-freetype /
    --disable-fribidi /
    --disable-fontconfig /
    --disable-libxml2 /
    --disable-hd1000v /
    --disable-skins2 /
    --disable-qt4 /
    --disable-atmo /
    /
    --disable-vlm /
    --disable-screen /
    --disable-wma-fixed /
    --disable-id3tag /
    --disable-fb /
    --disable-oss  /
    --disable-pulse /
    --disable-alsa  /
    --disable-upnp  /
    --disable-ncurses /
    --disable-visual /
    --disable-libgcrypt /
    --disable-vlc /
    /
    --disable-gnutls /
    --disable-remoteosd /
    --disable-libmpeg2 /
    --disable-vorbis /
    --disable-speex /
    --disable-schroedinger /
    --disable-png /
    --disable-x264 /
    --disable-fluidsynth /
    --disable-cmml /
    --disable-kate /
    --disable-tiger /
    --disable-bonjour /
    /
#    --disable-httpd /
#    --enable-static /
