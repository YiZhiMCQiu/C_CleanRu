#pragma once
#include<iostream>
#include<filesystem>
#include<vector>
#include<memory>
#include<map>
#include<set>
#include<algorithm>
#include<string>
#include "MengThreadPool.h"
using namespace std;
typedef string C_UserPath;
typedef filesystem::directory_entry fileDirEntry;

class MFileClean{
public:
	MFileClean();
	MFileClean(MThreadPool* tP, map<string, string>& tempSpList);
	MFileClean(MFileClean& mfile) = delete;
	void coutGetsFileList();
	void scanFile();
	static string getUserDir();
private:
	multimap<string, set<string>> getsFileList; //scanFile函数扫描到后获取的文件的地址;
	map<string, string> target_path;
	set<string> dirFile;
	int allFindNum = 0;
	C_UserPath path;
	fileDirEntry* user;
	MThreadPool* threadPool = nullptr;
};