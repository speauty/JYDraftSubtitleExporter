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
	bool m_FlagTargetFileReflush;
	bool m_FlagTargetFileForceOverride;

	Json::Reader m_JsonReader;
	Json::Value m_JsonData;

public:
	JYDraftSubtitleExporter();
	~JYDraftSubtitleExporter();

	bool SetSourceFilePath(const std::string& path);
	bool SetTargetFilePath(const std::string& path);
	void SetFlagHasTimeCode(bool flag);
	void SetFlagTargetFileForceOverride(bool flag);
	void DelTargetFile();
	bool ExecExport();

	std::string GetSourceFilePath() const { return m_SourceFilePath; }
	std::string GetTargetFilePath() const { return m_TargetFilePath; }
	bool GetFlagHasTimeCode() const { return m_FlagHasTimeCode; }
	unsigned int GetBlockCount() const { return m_BlockCount; }
};
