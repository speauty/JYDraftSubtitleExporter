### 剪映字幕导出工具-命令行版
导出剪映草稿字幕文件，主要有以下设置：
* 第一参数,  设置剪映草稿字幕文件路径;
* 第二参数, 设置转存文件路径(自动补充后缀srt);
* --force-override, 是否强制覆盖转存文件(如果已存在);

用例 `JYDraftSubtitleExporter.exe <draftPath> <exportPath> [--force-override]`
实例 `.\JYDraftSubtitleExporter.exe E:/draft.json E:/test`