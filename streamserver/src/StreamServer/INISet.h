#pragma once

//���ļ����ڴ洢�Զ��峣��
//�򲻿��Ƕ�����֧�֣���ʱ���ڴ��ļ���

//===================================�����ļ�====================================

//�����������ļ���
#define INIFILENAME ".\\streamserver.ini"

//ϵͳ��LED��������Ϣʱ���ɵ���ʱ�ļ���
#define TEMP_LED_SCREEN_TXT_NAME "add.txt"

//���Audio�豸��Ϣ���ļ���������������
#define AUDIO_XML_NAME "audio.xml"

//���led���豸��Ϣ���ļ���
#define LED_SCREEN_XML_NAME "ledscreen.xml"

//xml�ļ��и��ڵ�����
#define ISSEND_NODE_NAME "issend"
#define IP_NODE_NAME "ip"

#define AUDIO_PHONES_NODE_NAME "AudioPhones"
#define AUDIO_PHONE_NODE_NAME "audiophone"

#define LED_SCREENS_NODE_NAME "LedScreens"
#define LED_SCREEN_NODE_NAME "screen"
#define LED_ALERT_NODE_NAME "alert"
#define LED_CANCELALERT_NODE_NAME "cancelalert"


//====================================ϵͳ�趨===================================

//LED������
#define LED_SCREENS_FONT "����"

//�㲥Ĭ�϶˿�
#define DEFAULT_BROADCAST_PORT "5004"

//�㲥Ĭ�ϵ�ַ
#define DEFAULT_BROADCAST_ADDRESS "192.168.1.255"

//��̨ɨ���̼߳��ʱ��(ms)
#define SCAN_TIME_INTERVAL 5000

//��ʾ���ڱ�����
#define  WARNING_INFO "��ʾ"

//���Ű�ť����
#define PAUSE_INFO_TEXT "��ͣ"
#define PLAY_INFO_TEXT "����"

//������ť����
#define ALERT_INFO_TEXT "�������"
#define CANCEL_ALERT_INFO_TEXT "�����㲥"

//�趨������ʾ
#define RESET_UPDATE_INFO_TEXT "�趨���³ɹ�����������������ʹ�趨��Ч"

//��������ʱ����������
#define PROGRAM_RUN_FIRST_INFO_TEXT "�������Զ���ȡ������IP������udp�㲥��ʽ��ͻ����ṩ��ý�岥�ŷ���\r\n"


//=================================�Զ��������Ϣ���================================

//��LED�����һ����Ļ����
#define LED_APPEND_SCREEN_ERR "�����Ļ�����豸IP:"

//�������δ���
#define LED_SET_SCREEN_ATTR_ERR "�������δ����豸IP:"

//��ӽ�Ŀ����
#define LED_APPEND_PROGRAM_ERR "��ӽ�Ŀ�����豸IP:"

//����Ļ���һ��������ִ���
#define LED_APPEND_AREA_ERR "�����������豸IP:"

//��һ�����������Ϣ����
#define LED_APPEND_INFO_TO_AREA_ERR "���������Ϣ�����豸IP:"

//��ʼ����˷�ʧ��
#define INIT_AUDIO_INPUT_ERR "��ʼ����Ƶ�����豸ʧ�ܣ�������˷��Ƿ�����"

//���<����>��ť��������
#define CLICK_ALERT_BTN_TOO_MANY "������Ϣ�����벻ҪƵ�������ť!"

//�����豸���糬ʱ
#define CONNECT_NET_DEVICE_ERR "�޷����ӵ�ָ���豸�����������Ƿ�ͨ���豸IP:"

//����socket�ӿ�ʧ��
#define CREATE_SOCKET_ERR "�����豸�������⣬���������Ƿ�����"

//�豸��ʼ������
#define INIT_NET_ERR "������ʼ��ʧ�ܣ��������绷��"

//�����б�Ϊ��
#define PLAY_LIST_NULL_ERR "��������б����"