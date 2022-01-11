#include "Utils.h"

void MicroS2Str(std::string& msFmtStr, unsigned int microSeconds)
{
	/* 定义时分秒&毫秒 */
	unsigned int numH = 0, numM = 0, numS = 0, numMs = 0;
	char swap[4];
	unsigned int tmp;
	tmp = (unsigned int)(microSeconds / 1e3); /* 去掉微秒部分 */
	numMs = tmp % 1000; /* 转存毫秒部分 */
	tmp = (unsigned int)(tmp / 1e3); /* 去掉毫秒部分 */

	/* 计算时分秒 */
	numH = (unsigned int)(tmp / 3600);
	tmp -= numH * 3600;
	if (tmp) {
		numM = tmp / 60;
		numS = tmp - numM * 60;
	}

	msFmtStr = "";

	if (numH > 24) numH = 24;
	if (numH < 10) msFmtStr += '0';
	sprintf_s(swap, "%d", numH);
	msFmtStr += swap;
	msFmtStr += ":";
	if (numM < 10) msFmtStr += "0";
	sprintf_s(swap, "%d", numM);
	msFmtStr += swap;
	msFmtStr += ":";
	if (numS < 10) msFmtStr += "0";
	sprintf_s(swap, "%d", numS);
	msFmtStr += swap;
	msFmtStr += ",";

	if (numMs < 100) msFmtStr += "0";
	if (numMs < 10) msFmtStr += "0";
	sprintf_s(swap, "%d", numMs);
	msFmtStr += swap;

	return;
}
