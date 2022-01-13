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
		ERR_PRINT("���޲ݸ��ļ�");
		return false;
	}
	std::fstream tmp(path);
	if (!tmp.is_open()) {
		tmpStr.clear();
		tmpStr = "�ݸ��ļ����쳣[" + path + "]";
		ERR_PRINT(tmpStr);
		tmp.close(); return false;
	}
	
	if (m_JsonReader.parse(tmp, m_JsonData, true)) {

		if (m_JsonData.empty()) {
			tmpStr.clear();
			tmpStr = "�ݸ��ļ�JSON������������, �ݶ��Ƿ��ļ�[" + path + "]";
			ERR_PRINT(tmpStr);
			tmp.close(); return false;
		}

		m_SourceFilePath = path;
		tmp.close(); 
		return true;
	} else {
		tmpStr.clear();
		tmpStr = "�����쳣[" + path + "]";
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
		tmpStr = "ת���ļ��Ѵ���[" + path + "." + TARGET_EXT + "], ��ǰδ����ǿ�Ƹ���ѡ��[--force-override]";
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
		tmpStr = "ת���ļ����쳣[" + m_TargetFilePath + "]";
		ERR_PRINT(tmpStr);
		out.close(); return false;
	}
	m_FlagTargetFileReflush = true;
	
	std::ifstream source(m_SourceFilePath);

	if (!m_JsonReader.parse(source, m_JsonData)) {
		tmpStr.clear();
		tmpStr = "�ݸ��ļ�JSON�����쳣[" + m_SourceFilePath + "]";
		ERR_PRINT(tmpStr);
		source.close(); 
		out.close(); 
		DelTargetFile();
		return false;
	}

	if (m_JsonData.empty() || !m_JsonData.isMember("materials") || !m_JsonData["materials"].isMember("texts") || !m_JsonData.isMember("tracks")) {
		tmpStr.clear();
		tmpStr = "�ݸ��ļ������������������, �ݶ��Ƿ��ļ�[" + m_SourceFilePath + "]";
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
