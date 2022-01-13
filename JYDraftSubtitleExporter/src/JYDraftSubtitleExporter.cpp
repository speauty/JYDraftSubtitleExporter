#include "JYDraftSubtitleExporter.h"
#include <fstream>
#include "Utils.h"

std::string tmpStr = "";

JYDraftSubtitleExporter::JYDraftSubtitleExporter()
	:m_SourceFilePath(""), m_TargetFilePath(""), m_FlagHasTimeCode(true), m_BlockCount(0), m_FlagTargetFileForceOverride(false), m_FlagTargetFileReflush(false)
{
}

JYDraftSubtitleExporter::~JYDraftSubtitleExporter()
{
	m_SourceFilePath.clear();
	m_TargetFilePath.clear();
	m_BlockCount = 0;
	m_JsonReader = {};
	m_JsonData = NULL;
	m_FlagHasTimeCode = true;
	m_FlagTargetFileForceOverride = false;
	m_FlagTargetFileReflush = false;
}

bool JYDraftSubtitleExporter::SetSourceFilePath(const std::string& path)
{
	if (path.empty()) {
		ERR_PRINT("暂无草稿文件");
		return false;
	}
	std::fstream tmp(path);
	if (!tmp.is_open()) {
		tmpStr.clear();
		tmpStr = "草稿文件打开异常[" + path + "]";
		ERR_PRINT(tmpStr);
		tmp.close(); return false;
	}
	
	if (m_JsonReader.parse(tmp, m_JsonData, true)) {

		if (m_JsonData.empty()) {
			tmpStr.clear();
			tmpStr = "草稿文件JSON解析暂无数据, 暂定非法文件[" + path + "]";
			ERR_PRINT(tmpStr);
			tmp.close(); return false;
		}

		m_SourceFilePath = path;
		tmp.close(); 
		return true;
	} else {
		tmpStr.clear();
		tmpStr = "解析异常[" + path + "]";
		ERR_PRINT(tmpStr);
	}

	tmp.close();
	return false;
}

bool JYDraftSubtitleExporter::SetTargetFilePath(const std::string& path)
{
	if (path.empty()) return false;
	std::fstream tmp(path + "." + TARGET_EXT);
	if (tmp.is_open() && !m_FlagTargetFileForceOverride) {
		tmpStr.clear();
		tmpStr = "转存文件已存在[" + path + "." + TARGET_EXT + "], 当前未设置强制覆盖选项[--force-override]";
		ERR_PRINT(tmpStr);
		tmp.close(); return false;
	}
	tmp.close();
	m_TargetFilePath = path + "." + TARGET_EXT;
	return true;
}

void JYDraftSubtitleExporter::SetFlagHasTimeCode(bool flag)
{
	m_FlagHasTimeCode = flag;
	return;
}

void JYDraftSubtitleExporter::SetFlagTargetFileForceOverride(bool flag)
{
	m_FlagTargetFileForceOverride = flag;
	return;
}

void JYDraftSubtitleExporter::DelTargetFile()
{
	if (m_FlagTargetFileReflush && !m_TargetFilePath.empty()) remove(m_TargetFilePath.c_str());
}

bool JYDraftSubtitleExporter::ExecExport()
{
	std::ofstream out;
	out.open(m_TargetFilePath, std::ios::out|std::ios::trunc);
	if (!out.is_open()) {
		tmpStr.clear();
		tmpStr = "转存文件打开异常[" + m_TargetFilePath + "]";
		ERR_PRINT(tmpStr);
		out.close(); return false;
	}
	m_FlagTargetFileReflush = true;
	
	std::ifstream source(m_SourceFilePath);

	if (!m_JsonReader.parse(source, m_JsonData)) {
		tmpStr.clear();
		tmpStr = "草稿文件JSON解析异常[" + m_SourceFilePath + "]";
		ERR_PRINT(tmpStr);
		source.close(); 
		out.close(); 
		DelTargetFile();
		return false;
	}

	if (m_JsonData.empty() || !m_JsonData.isMember("materials") || !m_JsonData["materials"].isMember("texts") || !m_JsonData.isMember("tracks")) {
		tmpStr.clear();
		tmpStr = "草稿文件解析数据无相关索引, 暂定非法文件[" + m_SourceFilePath + "]";
		ERR_PRINT(tmpStr);
		source.close();
		out.close();
		DelTargetFile();
		return false;
	}

	unsigned int count = m_JsonData["materials"]["texts"].size();
	std::string timeStartSwap;
	std::string timeEndSwap;

	for (unsigned int i = 0; i < count; i++) {
		m_BlockCount++;
		std::string text = m_JsonData["materials"]["texts"][i]["content"].asString();
		unsigned int start = m_JsonData["tracks"][1]["segments"][i]["target_timerange"]["start"].asInt();
		unsigned int duration = m_JsonData["tracks"][1]["segments"][i]["target_timerange"]["duration"].asInt();
		MicroS2Str(timeStartSwap, start);
		MicroS2Str(timeEndSwap, start + duration);
		out << m_BlockCount << std::endl
			<< timeStartSwap << " --> " << timeEndSwap << std::endl
			<< text << std::endl
			<< std::endl;
	}

	source.close();
	out.close();

	return true;
}
