#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "LedCtrl.h"
#include "SockAudiosw.h"

using namespace std;

//���ú�����װ
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

	//����url�ַ�ת��
	inline unsigned char toHex(const unsigned char &x)
	{
		return x > 9 ? x -10 + 'A': x + '0';
	}

	inline unsigned char fromHex(const unsigned char &x)
	{
		return isdigit(x) ? x-'0' : x-'A'+10;
	}

	//�����������������·�������
	string EncodeUtf8(wstring in);
	wstring DecodeUtf8(string in);

	//urlת��
	string URLEncode(const string &sIn);
	string URLDecode(const string &sIn);

	unsigned int Alert(void* pParam);
	unsigned int AlertAudio(void* pParam);

	unsigned int CancelAlert(void* pParam);
	unsigned int CancelAlertAudio(void* pParam);

	//��̨ɨ���豸�Ƿ���ͨ
	void ScanAudioDevices(void);
	void ScanLedDevices(void);

	//��xml�ļ��ж�ȡscreen��Ϣ
	bool ReadScreenFromXmlFile(vector<led_screen> &screens);
	//��xml�ļ��ж�ȡҪ���Ƶ�audio phone�㲥��Ϣ
	bool ReadAudioFromXmlFile(vector<audio_phone> &audioPhones);
	//��xml�ļ���д���豸��Ϣ
	bool WriteAudioPhoneXmlFile(const vector<audio_phone> &audioPhones);
	bool WriteScreensXmlFile(const vector<led_screen> &screens);

	//��ȡ��ǰ��������·��
	void GetCurrentExecPath(char * fileName,char* filePath);
};
