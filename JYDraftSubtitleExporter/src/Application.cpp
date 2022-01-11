#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "JYDraftSubtitleExporter.h"

/**
 * 解析剪映草稿字幕应用
 * 
 * 字幕稿 materials.texts[].content
 * 时间线 tracks[].segments[].target_timerange.(start-duration)
 **/

int main()
{
	SetConsoleTitle(_T("剪映字幕导出-极速版"));

	JYDraftSubtitleExporter exporter;
	std::string tmpString;
	unsigned int timeStart = GetTickCount64();

	do {
		tmpString.clear();
		std::cout << "请输入待解析的剪映草稿文件路径: ";
		std::cin >> tmpString;
		if (!exporter.SetSourceFilePath(tmpString)) {
			std::cout << "草稿文件解析异常" << std::endl;
		} else {
			break;
		}
	} while (1);
	do {
		tmpString.clear();
		std::cout << "请输入转存文件: ";
		std::cin >> tmpString;
		if (!exporter.SetTargetFilePath(tmpString)) {
			std::cout << "转存文件解析异常" << std::endl;
		}
		else {
			break;
		}
	} while (1);

	if (!exporter.ExecExport()) {
		std::cout << "导出字幕文件异常" << std::endl;
		return -1;
	}
	std::cout << "导出字幕文件成功" << std::endl;

	system("pause");
}