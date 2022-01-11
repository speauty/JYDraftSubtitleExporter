#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "JYDraftSubtitleExporter.h"

/**
 * ������ӳ�ݸ���ĻӦ��
 * 
 * ��Ļ�� materials.texts[].content
 * ʱ���� tracks[].segments[].target_timerange.(start-duration)
 **/

int main()
{
	SetConsoleTitle(_T("��ӳ��Ļ����-���ٰ�"));

	JYDraftSubtitleExporter exporter;
	std::string tmpString;
	unsigned int timeStart = GetTickCount64();

	do {
		tmpString.clear();
		std::cout << "������������ļ�ӳ�ݸ��ļ�·��: ";
		std::cin >> tmpString;
		if (!exporter.SetSourceFilePath(tmpString)) {
			std::cout << "�ݸ��ļ������쳣" << std::endl;
		} else {
			break;
		}
	} while (1);
	do {
		tmpString.clear();
		std::cout << "������ת���ļ�: ";
		std::cin >> tmpString;
		if (!exporter.SetTargetFilePath(tmpString)) {
			std::cout << "ת���ļ������쳣" << std::endl;
		}
		else {
			break;
		}
	} while (1);

	if (!exporter.ExecExport()) {
		std::cout << "������Ļ�ļ��쳣" << std::endl;
		return -1;
	}
	std::cout << "������Ļ�ļ��ɹ�" << std::endl;

	system("pause");
}