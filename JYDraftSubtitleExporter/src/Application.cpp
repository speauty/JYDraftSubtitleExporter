#include <iostream>
#include <vector>
#include <windows.h>
#include "Lib/clipp.h"
#include "Utils.h"
#include "JYDraftSubtitleExporter.h"

/**
 * 解析剪映草稿字幕应用
 * 
 * 字幕稿 materials.texts[].content
 * 时间线 tracks[].segments[].target_timerange.(start-duration)
 **/

int main(int argc, char* argv[])
{
	std::string draftPath = "", exportPath = "";
	bool flagHasTimeCode = true;
	bool flagTargetFileForceOverride = false;
	unsigned long long startTime = GetTickCount64();
	std::vector<std::string> wrong;
	auto cli = (
		clipp::value("draftPath", draftPath).required(true).doc("设置剪映草稿字幕文件路径;")
		.if_missing([] { ERR_PRINT("暂未检测到剪映草稿字幕文件路径"); }),
		clipp::value("exportPath", exportPath).required(true).doc("设置转存文件路径(自动补充后缀srt);")
			.if_missing([] { ERR_PRINT("暂未检测到转存文件路径"); }),
		clipp::option("--force-override").set(flagTargetFileForceOverride, true).doc("是否强制覆盖转存文件(如果已存在);"),
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
		std::cout << "导出字幕文件成功[" << exporter.GetTargetFilePath() << "], 字幕块: " << exporter.GetBlockCount() << ", 耗时(ms): " << (GetTickCount64() - startTime) << std::endl;

	} else {
		for (const auto& arg : wrong) {
			ERR_PRINT(arg + "非法参数");
		}
		std::cerr << clipp::make_man_page(cli, argv[0]) << std::endl;
	}
}