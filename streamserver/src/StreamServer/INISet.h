#pragma once

//本文件用于存储自定义常量
//因不考虑多语言支持，暂时放在此文件中

//===================================配置文件====================================

//主程序配置文件名
#define INIFILENAME ".\\streamserver.ini"

//系统向LED屏发送信息时生成的临时文件名
#define TEMP_LED_SCREEN_TXT_NAME "add.txt"

//存放Audio设备信息的文件名，即主开发板
#define AUDIO_XML_NAME "audio.xml"

//存放led屏设备信息的文件名
#define LED_SCREEN_XML_NAME "ledscreen.xml"

//xml文件中各节点名称
#define ISSEND_NODE_NAME "issend"
#define IP_NODE_NAME "ip"

#define AUDIO_PHONES_NODE_NAME "AudioPhones"
#define AUDIO_PHONE_NODE_NAME "audiophone"

#define LED_SCREENS_NODE_NAME "LedScreens"
#define LED_SCREEN_NODE_NAME "screen"
#define LED_ALERT_NODE_NAME "alert"
#define LED_CANCELALERT_NODE_NAME "cancelalert"


//====================================系统设定===================================

//LED屏字体
#define LED_SCREENS_FONT "宋体"

//广播默认端口
#define DEFAULT_BROADCAST_PORT "5004"

//广播默认地址
#define DEFAULT_BROADCAST_ADDRESS "192.168.1.255"

//后台扫描线程间隔时间(ms)
#define SCAN_TIME_INTERVAL 5000

//提示窗口标题栏
#define  WARNING_INFO "提示"

//播放按钮文字
#define PAUSE_INFO_TEXT "暂停"
#define PLAY_INFO_TEXT "播放"

//警报按钮文字
#define ALERT_INFO_TEXT "解除警报"
#define CANCEL_ALERT_INFO_TEXT "紧急广播"

//设定更新提示
#define RESET_UPDATE_INFO_TEXT "设定更新成功，您必须重启程序使设定生效"

//初次启动时程序功能描述
#define PROGRAM_RUN_FIRST_INFO_TEXT "本程序自动获取服务器IP，并以udp广播方式向客户端提供流媒体播放服务。\r\n"


//=================================自定义错误信息输出================================

//向LED屏添加一个屏幕错误
#define LED_APPEND_SCREEN_ERR "添加屏幕错误，设备IP:"

//设置屏参错误
#define LED_SET_SCREEN_ATTR_ERR "设置屏参错误，设备IP:"

//添加节目错误
#define LED_APPEND_PROGRAM_ERR "添加节目错误，设备IP:"

//向屏幕添加一个区域出现错误
#define LED_APPEND_AREA_ERR "添加区域错误，设备IP:"

//向一个区域添加信息错误
#define LED_APPEND_INFO_TO_AREA_ERR "区域添加信息错误，设备IP:"

//初始化麦克风失败
#define INIT_AUDIO_INPUT_ERR "初始化音频输入设备失败，请检查麦克风是否正常"

//点击<报警>按钮次数过多
#define CLICK_ALERT_BTN_TOO_MANY "发送信息错误，请不要频繁点击按钮!"

//连接设备网络超时
#define CONNECT_NET_DEVICE_ERR "无法连接到指定设备，请检查网络是否畅通，设备IP:"

//建立socket接口失败
#define CREATE_SOCKET_ERR "网络设备出现问题，请检查网卡是否正常"

//设备初始化问题
#define INIT_NET_ERR "网卡初始化失败，请检查网络环境"

//歌曲列表为空
#define PLAY_LIST_NULL_ERR "请先添加列表歌曲"