#include <iostream>
#include <vector>
#include <windows.h>
#include "Lib/clipp.h"
#include "Utils.h"
#include "JYDraftSubtitleExporter.h"

/**
 * ������ӳ�ݸ���ĻӦ��
 * 
 * ��Ļ�� materials.texts[].content
 * ʱ���� tracks[].segments[].target_timerange.(start-duration)
 **/

int main(int argc, char* argv[])
{
	std::string draftPath = "", exportPath = "";
	bool flagHasTimeCode = true;
	bool flagTargetFileForceOverride = false;
	unsigned long long startTime = GetTickCount64();
	std::vector<std::string> wrong;
	auto cli = (
		clipp::value("draftPath", draftPath).required(true).doc("���ü�ӳ�ݸ���Ļ�ļ�·��;")
		.if_missing([] { ERR_PRINT("��δ��⵽��ӳ�ݸ���Ļ�ļ�·��"); }),
		clipp::value("exportPath", exportPath).required(true).doc("����ת���ļ�·��(�Զ������׺srt);")
			.if_missing([] { ERR_PRINT("��δ��⵽ת���ļ�·��"); }),
		clipp::option("--force-override").set(flagTargetFileForceOverride, true).doc("�Ƿ�ǿ�Ƹ���ת���ļ�(����Ѵ���);"),
		clipp::any_other(wrong)
	);
	auto res = parse(argc, argv, cli);
	if (res && wrong.empty()) {
		JYDraftSubtitleExporter exporter;
		exporter.SetFlagHasTimeCode(flagHasTimeCode);
		exporter.SetFlagTargetFileForceOverride(flagTargetFileForceOverride);
		if (!exporter.SetSourceFilePath(draftPath) || !exporter.SetTargetFilePath(exportPath) || !exporter.ExecExport()) {
			return -1;
		}
		std::cout << "������Ļ�ļ��ɹ�[" << exporter.GetTargetFilePath() << "], ��Ļ��: " << exporter.GetBlockCount() << ", ��ʱ(ms): " << (GetTickCount64() - startTime) << std::endl;

	} else {
		for (const auto& arg : wrong) {
			ERR_PRINT(arg + "�Ƿ�����");
		}
		std::cerr << clipp::make_man_page(cli, argv[0]) << std::endl;
	}
}