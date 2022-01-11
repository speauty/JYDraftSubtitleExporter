#include "JYDraftSubtitleExporter.h"
#include <fstream>
#include "Utils.h"

JYDraftSubtitleExporter::JYDraftSubtitleExporter()
	:m_SourceFilePath(""), m_TargetFilePath(""), m_FlagHasTimeCode(true), m_BlockCount(0)
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
}

bool JYDraftSubtitleExporter::SetSourceFilePath(const std::string& path)
{
	if (path.empty()) return false;
	std::fstream tmp(path);
	if (!tmp.is_open())
	{
		tmp.close(); return false;
	}
	
	if (m_JsonReader.parse(tmp, m_JsonData, true))
	{
		m_SourceFilePath = path;
		tmp.close(); 
		return true;
	}

	tmp.close();
	return false;
}

bool JYDraftSubtitleExporter::SetTargetFilePath(const std::string& path)
{
	if (path.empty()) return false;
	std::fstream tmp(path + "." + TARGET_EXT);
	if (tmp.is_open())
	{
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

bool JYDraftSubtitleExporter::ExecExport()
{
	std::ofstream out;
	out.open(m_TargetFilePath, std::ios::out|std::ios::trunc);
	if (!out.is_open())
	{
		out.close(); return false;
	}
	
	std::ifstream source(m_SourceFilePath);

	if (!m_JsonReader.parse(source, m_JsonData))
	{
		source.close(); out.close(); return false;
	}

	if (!m_JsonData.isMember("materials") || !m_JsonData["materials"].isMember("texts") || !m_JsonData.isMember("tracks"))
	{
		source.close(); out.close(); return false;
	}

	unsigned int count = m_JsonData["materials"]["texts"].size();
	std::string timeStartSwap;
	std::string timeEndSwap;

	for (unsigned int i = 0; i < count; i++)
	{
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
