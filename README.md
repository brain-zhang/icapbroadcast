icapbroadcast
=============

矿用环网广播系统

------------------
目前矿用广播系统还停留在原始的功放+电缆的部署阶段。

缺点:
    安装成本高，每台终端都要大量线缆延伸到广播点，部署一台功放一般报价很高
    维护麻烦，因为线缆太多，很容易中途损害
    安装完成后，如果广播点需要移动，费时费力
    在某些安装点(如工作面)，因为煤尘大，回采环境复杂，安装点每天都有变动，很难稳定运行。

icapbroadcase利用现有的井下以太环网，结合矿用本安无线AP,可以灵活选择安装点，轻松解决了以上问题。

------------------
实现原理

icapbroadcase采用 "井上流媒体服务器+井下环网+无线AP+移动终端"的部署方式

     井上部分:一台连入井下环网的服务器，运行streamserver进行流媒体广播(VLC )
               ||
               --
     通讯部分:现有的以太环网
               ||
               --
          井下(无线AP)
           /    |     \
      播放终端  播放终端 播放终端

播放终端采用s3c2440 嵌入式平台


优点:
    成本低,充分利用现有的以太环网，无需部署专用功放机。井下采用防爆本安外壳放置AP及终端。
    部署灵活，机房就近部署无线AP，播放终端可以安置在任何12V供电处。

------------------
Resource:
.
|-- arm-linux-vlc (arms3c2440 VLC移植. 环境:arm-linux-GCC4.3.3 Fedora16 vlc1.1.3)
|   |-- bin
|   `-- src
`-- streamserver                            井上流媒体服务器demo, 开发环境vs2010
    `-- src                                 用于向客户端广播，目前支持udp协议。
        |-- StreamServer                    程序采用了libvlc作为流媒体服务核心库。
        |   |-- Debug
        |   |-- Release
        |   |-- libvlcwrapper
        |   |-- res
        |   `-- tinixml
        |-- sdk
        |   |-- include
        |   |   `-- vlc
        |   |       `-- plugins
        |   `-- lib
        |       `-- pkgconfig
        `-- setup

使用方法:
1、编译运行StreamServer.exe
2、添加要播放的歌曲文件到列表
3、点击播放按钮

客户点播放测试:
1、打开mediaplayer
2、文件->打开URL
3、输入:mms://服务器IP:8080  如(mms://192.168.1.1:8080)


