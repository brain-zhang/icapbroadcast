#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "LedCtrl.h"
#include "SockAudiosw.h"

using namespace std;

//常用函数封装
namespace CUtil
{
	struct led_screen{
		string ip;
		string alert;
		string cancelalert;
		string issend;
		string id;
	};

	struct audio_phone{
		string ip;
		string issend;
		string id;
	};

	//用于url字符转义
	inline unsigned char toHex(const unsigned char &x)
	{
		return x > 9 ? x -10 + 'A': x + '0';
	}

	inline unsigned char fromHex(const unsigned char &x)
	{
		return isdigit(x) ? x-'0' : x-'A'+10;
	}

	//辅助函数，解决中文路径的输出
	string EncodeUtf8(wstring in);
	wstring DecodeUtf8(string in);

	//url转义
	string URLEncode(const string &sIn);
	string URLDecode(const string &sIn);

	unsigned int Alert(void* pParam);
	unsigned int AlertAudio(void* pParam);

	unsigned int CancelAlert(void* pParam);
	unsigned int CancelAlertAudio(void* pParam);

	//后台扫描设备是否连通
	void ScanAudioDevices(void);
	void ScanLedDevices(void);

	//从xml文件中读取screen信息
	bool ReadScreenFromXmlFile(vector<led_screen> &screens);
	//从xml文件中读取要控制的audio phone广播信息
	bool ReadAudioFromXmlFile(vector<audio_phone> &audioPhones);
	//向xml文件中写入设备信息
	bool WriteAudioPhoneXmlFile(const vector<audio_phone> &audioPhones);
	bool WriteScreensXmlFile(const vector<led_screen> &screens);

	//获取当前程序运行路径
	void GetCurrentExecPath(char * fileName,char* filePath);
};
