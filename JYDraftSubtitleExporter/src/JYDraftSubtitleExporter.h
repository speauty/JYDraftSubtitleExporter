#pragma once

#include <iostream>
#include <json/json.h>

#define TARGET_EXT "srt"

class JYDraftSubtitleExporter
{
private:
	std::string m_SourceFilePath;
	std::string m_TargetFilePath;
	unsigned int m_BlockCount;
	bool m_FlagHasTimeCode;

	Json::Reader m_JsonReader;
	Json::Value m_JsonData;

public:
	JYDraftSubtitleExporter();
	~JYDraftSubtitleExporter();

	bool SetSourceFilePath(const std::string& path);
	bool SetTargetFilePath(const std::string& path);
	void SetFlagHasTimeCode(bool flag);
	bool ExecExport();

	std::string GetSourceFilePath() { return m_SourceFilePath; }
	std::string GetTargetFilePath() { return m_TargetFilePath; }
	bool GetFlagHasTimeCode() { return m_FlagHasTimeCode; }
};
