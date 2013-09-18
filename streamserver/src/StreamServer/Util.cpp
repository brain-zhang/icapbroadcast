#include "stdafx.h"
#include "windows.h"
#include "tchar.h"

#include "../tinixml/tinyxml.h"
#include "NetUtil.h"
#include "SSExcept.h"
#include "Util.h"
#include "INISet.h"

string CUtil::EncodeUtf8(wstring in)
{
	string s(in.length() * 3, ' ');  
	size_t len = ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), in.length(), &s[0], s.length(), NULL, NULL);  
	s.resize(len);  
	return s;
}

wstring CUtil::DecodeUtf8(string in)  
{  
	wstring s(in.length(), _T(' '));  
	size_t len = ::MultiByteToWideChar(CP_UTF8, 0, in.c_str(), in.length(), &s[0],s.length());  
	s.resize(len);  
	return s;  
}

string CUtil::URLEncode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{      
		unsigned char buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (unsigned char)sIn[ix] ) )
		{      
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (unsigned char)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
		//{
		//    buf[0] = '+';
		//}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
			buf[2] = toHex( (unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
};

string CUtil::URLDecode(const string &sIn)
{
	string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		unsigned char ch = 0;
		if(sIn[ix]=='%')
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}

unsigned int CUtil::Alert(void* pParam)
{
	//控制LED
	CLedCtrl *led = static_cast<CLedCtrl *>(pParam);
	vector<CUtil::led_screen> vscreens;
	CUtil::ReadScreenFromXmlFile(vscreens);
	for (vector<CUtil::led_screen>::iterator iter = vscreens.begin(); iter != vscreens.end(); iter++)
	{
		try
		{
			if ((*iter).issend == string("True"))
			{
				led->PrepareMessage((*iter).ip, (*iter).alert, (*iter).id);
				led->SendMessage((*iter).id);
			}
		}

		catch(std::exception &e)
		{
			CString s(e.what());
			AfxMessageBox(s);
		}
	}

	return 0;
}


unsigned int CUtil::AlertAudio(void* pParam)
{
	CSockAudiosw *audio = static_cast<CSockAudiosw *>(pParam);
	vector<CUtil::audio_phone> audioPhones;
	CUtil::ReadAudioFromXmlFile(audioPhones);
	for (vector<CUtil::audio_phone>::iterator iter = audioPhones.begin(); iter != audioPhones.end(); iter++)
	{
		try
		{
			if ((*iter).issend == string("True"))
			{
				audio->SendMessage((*iter).ip, string("enable"));
			}
		}

		catch(std::exception &e)
		{
			CString s(e.what());
			AfxMessageBox(s);
		}
	}
	return 0;
}

//取消报警
unsigned int CUtil::CancelAlert(void* pParam)
{
	try
	{
		CLedCtrl *led = static_cast<CLedCtrl *>(pParam);
		vector<CUtil::led_screen> vscreens;
		CUtil::ReadScreenFromXmlFile(vscreens);
		for (vector<CUtil::led_screen>::iterator iter = vscreens.begin(); iter != vscreens.end(); iter++)
		{
			if ((*iter).issend == string("True"))
			{
				led->PrepareMessage((*iter).ip, (*iter).cancelalert, (*iter).id);
				led->SendMessage((*iter).id);
			}
		}
	}

	catch(CLedExcept &e)
	{
		CString s(e.what());
		AfxMessageBox(s);
	}
	return 0;
}

unsigned int CUtil::CancelAlertAudio(void* pParam)
{
	try
	{
		CSockAudiosw *audio = static_cast<CSockAudiosw *>(pParam);
		vector<CUtil::audio_phone> audioPhones;
		CUtil::ReadAudioFromXmlFile(audioPhones);
		for (vector<CUtil::audio_phone>::iterator iter = audioPhones.begin(); iter != audioPhones.end(); iter++)
		{
			if ((*iter).issend == string("True"))
			{
				audio->SendMessage((*iter).ip, string("disable"));
			}
		}
	}

	catch(std::exception &e)
	{
		CString s(e.what());
		AfxMessageBox(s);
	}
	return 0;
}

//读取Screens的设定信息
bool CUtil::ReadScreenFromXmlFile(vector<led_screen> &screens)
{//读取Xml文件，并遍历
	try
	{
		char path[1024] = {0};
		GetCurrentExecPath(LED_SCREEN_XML_NAME, path);
		//创建一个XML的文档对象。
		TiXmlDocument myDocument;
		myDocument.LoadFile(path);
		//获得根元素，即LedScreens。
		TiXmlElement *RootElement = myDocument.RootElement();
		//获得第一个LedScreen节点。
		TiXmlElement *screenElement = RootElement->FirstChildElement();
		while (screenElement)
		{
			//获得第一个LedScreen的ip节点和alert节点、cancelalert节点和ID属性。
			TiXmlElement *issendElement = screenElement->FirstChildElement();
			TiXmlElement *ipElement = issendElement->NextSiblingElement();
			TiXmlElement *alertElement = ipElement->NextSiblingElement();
			TiXmlElement *cancelalertElement = alertElement->NextSiblingElement();
			TiXmlAttribute *IDAttribute = screenElement->FirstAttribute();
			screenElement = screenElement->NextSiblingElement();

			led_screen screen;
			screen.ip = string(ipElement->FirstChild()->Value());
			screen.alert = string(alertElement->FirstChild()->Value());
			screen.cancelalert = string(cancelalertElement->FirstChild()->Value());
			screen.issend = string(issendElement->FirstChild()->Value());
			screen.id = string(IDAttribute->Value());
			screens.push_back(screen);
		}
	}
	catch (string& e)
	{
		throw e;
		return false;
	}
	return true;
}

//读取Audio Phone的设定信息
bool CUtil::ReadAudioFromXmlFile(vector<audio_phone> &audioPhones)
{//读取Xml文件，并遍历
	try
	{
		char path[1024] = {0};
		GetCurrentExecPath(AUDIO_XML_NAME, path);
		//创建一个XML的文档对象。
		TiXmlDocument myDocument;
		myDocument.LoadFile(path);
		//获得根元素，即AudioPhones。
		TiXmlElement *RootElement = myDocument.RootElement();
		//获得第一个audiophone节点。
		TiXmlElement *audioElement = RootElement->FirstChildElement();
		while (audioElement)
		{
			//获得第一个audiophone的ip节点和issend节点和ID属性。
			TiXmlElement *issendElement = audioElement->FirstChildElement();
			TiXmlElement *ipElement = issendElement->NextSiblingElement();
			TiXmlAttribute *IDAttribute = audioElement->FirstAttribute();
			audioElement = audioElement->NextSiblingElement();

			audio_phone audio;
			audio.ip = string(ipElement->FirstChild()->Value());
			audio.issend = string(issendElement->FirstChild()->Value());
			audio.id = string(IDAttribute->Value());
			audioPhones.push_back(audio);
		}
	}
	catch (string& e)
	{
		throw e;
		return false;
	}
	return true;
}


//设置Auido Phone的信息
bool CUtil::WriteAudioPhoneXmlFile(const vector<audio_phone> &audioPhones)
{
	try
	{
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument();
		//创建一个根元素并连接。
		TiXmlElement *RootElement = new TiXmlElement(AUDIO_PHONES_NODE_NAME);
		myDocument->LinkEndChild(RootElement);
		int i = 0;
		for (vector<audio_phone>::const_iterator iter = audioPhones.begin(); iter < audioPhones.end(); iter++, i++)
		{
			//创建一个audiophone元素并连接。
			TiXmlElement *audioElement = new TiXmlElement(AUDIO_PHONE_NODE_NAME);
			RootElement->LinkEndChild(audioElement);
			//设置audio元素的属性。
			audioElement->SetAttribute("id", (*iter).id.c_str());
			//创建issend元素、ip元素并连接。
			TiXmlElement *issendElement = new TiXmlElement(ISSEND_NODE_NAME);
			TiXmlElement *ipElement = new TiXmlElement(IP_NODE_NAME);
			audioElement->LinkEndChild(issendElement);
			audioElement->LinkEndChild(ipElement);
			//设置issend元素和ip元素的内容并连接。
			TiXmlText *issendContent = new TiXmlText((*iter).issend.c_str());
			TiXmlText *ipContent = new TiXmlText((*iter).ip.c_str());
			issendElement->LinkEndChild(issendContent);
			ipElement->LinkEndChild(ipContent);
		}

		char path[1024] = {0};
		GetCurrentExecPath(AUDIO_XML_NAME, path);
		myDocument->SaveFile(path);//保存到文件
		delete myDocument;
	}
	catch (string& e)
	{
		throw e;
		return false;
	}
	return true;
}


//设置Screens的设定信息
bool CUtil::WriteScreensXmlFile(const vector<led_screen> &screens)
{
	try
	{
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument();
		//创建一个根元素并连接。
		TiXmlElement *RootElement = new TiXmlElement(LED_SCREENS_NODE_NAME);
		myDocument->LinkEndChild(RootElement);
		for (vector<led_screen>::const_iterator iter = screens.begin(); iter < screens.end(); iter++)
		{
			//创建一个screen元素并连接。
			TiXmlElement *screenElement = new TiXmlElement(LED_SCREEN_NODE_NAME);
			RootElement->LinkEndChild(screenElement);
			//设置screen元素的属性。
			screenElement->SetAttribute("id", (*iter).id.c_str());

			//创建issend元素、ip元素并连接。
			TiXmlElement *issendElement = new TiXmlElement(ISSEND_NODE_NAME);
			TiXmlElement *ipElement = new TiXmlElement(IP_NODE_NAME);
			//警告信息
			TiXmlElement *alertElement = new TiXmlElement(LED_ALERT_NODE_NAME);
			TiXmlElement *cancelalertElement = new TiXmlElement(LED_CANCELALERT_NODE_NAME);
			//设置issend元素和ip元素的内容并连接。
			TiXmlText *issendContent = new TiXmlText((*iter).issend.c_str());
			TiXmlText *ipContent = new TiXmlText((*iter).ip.c_str());
			TiXmlText *alertContent = new TiXmlText((*iter).alert.c_str());
			TiXmlText *cancelalertContent = new TiXmlText((*iter).cancelalert.c_str());
			issendElement->LinkEndChild(issendContent);
			ipElement->LinkEndChild(ipContent);
			alertElement->LinkEndChild(alertContent);
			cancelalertElement->LinkEndChild(cancelalertContent);

			screenElement->LinkEndChild(issendElement);
			screenElement->LinkEndChild(ipElement);
			screenElement->LinkEndChild(alertElement);
			screenElement->LinkEndChild(cancelalertElement);
		}

		char path[1024] = {0};
		GetCurrentExecPath(LED_SCREEN_XML_NAME, path);
		myDocument->SaveFile(path);//保存到文件
		delete myDocument;
	}
	catch (string& e)
	{
		throw e;
		return false;
	}
	return true;
}

//扫描所有Audio 板子，如果不能连接，写入xml文件中
void CUtil::ScanAudioDevices()
{
	vector<audio_phone> vaudioPhones;
	ReadAudioFromXmlFile(vaudioPhones);

	for (vector<CUtil::audio_phone>::iterator iter = vaudioPhones.begin(); iter != vaudioPhones.end(); iter++)
	{
		if(!CNetUtil::Ping((*iter).ip.c_str()))
		{
			//Ping不通
			(*iter).issend = "False";
		}
		else
		{
			(*iter).issend = "True";
		}
	}
	WriteAudioPhoneXmlFile(vaudioPhones);
}

//扫描所有led 板子，如果不能连接，写入xml文件中
void CUtil::ScanLedDevices()
{
	//控制LED
	vector<CUtil::led_screen> vscreens;
	CUtil::ReadScreenFromXmlFile(vscreens);
	for (vector<CUtil::led_screen>::iterator iter = vscreens.begin(); iter != vscreens.end(); iter++)
	{
		if(!CNetUtil::Ping((*iter).ip.c_str()))
		{
			//Ping不通
			(*iter).issend = "False";
		}
		else
		{
			(*iter).issend = "True";
		}
	}
	WriteScreensXmlFile(vscreens);
}

//获取当前程序路径+文件名
void CUtil::GetCurrentExecPath(char * fileName,char* filePath)
{
	ASSERT(fileName != NULL);
	char path[1024];
	memset(path, 0, sizeof(path));
	::GetModuleFileNameA(NULL,path,sizeof(path));

	int strLenth=strlen(path);
	int i = 1;	//从字符串后面开始遍历，直到遇到'\'
	while(path[strLenth-i] != '\\')
	{
		path[strLenth-i]='\0';
		i++;
	}

	strcpy(filePath,path);
	strcat(filePath,fileName);
}
