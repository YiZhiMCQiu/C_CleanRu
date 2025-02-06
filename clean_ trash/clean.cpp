#include<iostream>
#include<Thread>
#include<map>

#include <windows.h>
#include <shlobj.h>

#include"system_clean.h"
#include"MengFileManager.h"
using namespace std;
typedef unsigned int choies;
namespace fs = filesystem;
map<string, string> target_path;

void reFile(string path) {
	try {
			if ((fs::status(path).permissions() & fs::perms::owner_read) != fs::perms::none) {
				for (auto& e : fs::directory_iterator(path)) {
					if (e.is_directory()) {
						reFile(e.path().string());
					}
					else if (e.is_regular_file()) {
						cout << e.path() << endl;
					}

				}
			}
	}
	catch (const fs::filesystem_error& e) {
		if (e.code() == errc::permission_denied) {
			cerr << "无访问权限:" << e.path1() << endl;
		}
		else {
			cerr << "未知错误,读取文件失败" << e.path1() << '\n';
		}
	}
}
void testFunc(string path, MThreadPool& threadPool) {

	threadPool.enterTask([&] {
		for (auto& e : filesystem::directory_iterator(path)) {
			if (e.is_directory()) {
				int index = e.path().string().rfind("\\");
				string dirName = e.path().string().substr(index + 1, e.path().string().length());
				if (dirName.compare("Microsoft") != 0) {
					reFile(e.path().string());
				}
			}
			else {
				cout << e.path() << endl;
			}
		}
		});

	if (threadPool.waitTaskComplete()) {
		cout << "the end..." << endl;
	}
	this_thread::sleep_for(chrono::seconds(1));
}

void fileRemoveTest() {
	for (auto& e : fs::directory_iterator("C:\\Users\\33953\\Downloads")) {
		if (fs::remove(e.path())) {
            cout << "删除成功" << endl;
		}
	}
}
map<string, string> addTarget();

int main() {
	/*target_path.insert(pair<string, string>("垃圾缓存文件(推荐删除)", "C:\\Users\\33953\\Downloads"));
	target_path.insert(pair<string, string>("临时互联网文件01,浏览器缓存文件(推荐删除)", "C:\\ProgramData\\Microsoft\\Windows\\WER"));
	target_path.insert(pair<string, string>("临时互联网文件02,浏览器缓存文件(推荐删除)", "C:\\Windows\\SoftwareDistribution\\Download"));
	target_path.insert(pair<string, string>("Windows更新缓存(推荐删除)", "C:\\Users\\33953\\AppData\\Local\\Temp"));
	target_path.insert(pair<string, string>("系统还原点(谨慎删除)", "C:\\Users\\33953\\AppData\\Local\\Microsoft\\Windows\\INetCache"));
	target_path.insert(pair<string, string >("应用程序缓存01(你最好是手动删awa,有些删了能清理空间,有些删了会影响程序正常运行 ╰(艹皿艹 ) (谨慎删除) ", "C:\\Users\\33953\\AppData\\Local\\Microsoft\\Windows\\WebCache"));
	target_path.insert(pair<string, string>("应用程序缓存02(谨慎删除)", "C:\\System Volume Information"));
	target_path.insert(pair<string, string>("Windows 错误报告(推荐删除)", "C:\\Users\\33953\\AppData\\Local"));
	target_path.insert(pair<string, string>("DirectX 着色器缓存(不推荐/谨慎删除)0.0", "C:\\Users\\33953\\AppData\\Roaming"));*/

	MThreadPool threadPool(12);
	map<string,string> target = addTarget();
	MScreen meng(&threadPool,target);

	choies cs;
	while (true) {
		system("cls");
		meng.showMenu();
		cin >> cs;

		switch (cs) {
		case 1:
			cout << "将在1s后开始清理..." << '\n';

			//cout << fs::file_size("C:\\Users\\33953\\Downloads\\新建文本文档.txt") << endl;
			meng.startClean();
			//testFunc("C:\\Windows\\SoftwareDistribution\\Download", threadPool);
			//fileRemoveTest();
			this_thread::sleep_for(chrono::seconds(30));
			system("cls");
			break;

		case 2:
			meng.showDetails();
			system("pause");
			break;
		}

	}
}


string wConvertString(const wstring& wstr) {
	if (wstr.empty()) {
		return "";
	}
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
	return strTo;

}

void convertT() {
	PWSTR pathPtr;
	wstring path;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &pathPtr);

	if (SUCCEEDED(hr)) {
		path = pathPtr;
		CoTaskMemFree(pathPtr);
	}
	else {
		std::cerr << "没找到。" << std::endl;
	}
	string user_path = wConvertString(path);
}

map<string, string> addTarget()
{
	PWSTR pathPtr;
	wstring path;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &pathPtr);

	if (SUCCEEDED(hr)) {
		path = pathPtr;
		CoTaskMemFree(pathPtr);
	}
	else {
		std::cerr << "没找到。" << std::endl;
	}
	string user_path = wConvertString(path);
	

	map<string, string> target;
	string userPath = MFileClean::getUserDir();
	if (filesystem::exists(userPath + "\\Downloads")) {
		target.insert(pair<string, string>("C盘下载文件夹(谨慎删除)", user_path + "\\Downloads"));
	}
	target.insert(pair<string, string>("垃圾缓存文件(推荐删除)", user_path + "\\AppData\\Local\\Temp"));
	target.insert(pair<string, string>("预取文件,加快程序启动速度缓存文件(谨慎删除)", "C:\\Windows\\prefetch"));
	target.insert(pair<string, string>("临时互联网文件01,浏览器缓存文件(谨慎删除)", user_path + "\\AppData\\Local\\Microsoft\\Windows\\INetCache"));
	target.insert(pair<string, string>("临时互联网文件02,浏览器缓存文件(谨慎删除)", user_path + "\\AppData\\Local\\Microsoft\\Windows\\WebCache"));
	target.insert(pair<string, string>("Windows更新缓存(推荐删除)", "C:\\Windows\\SoftwareDistribution\\Download"));
	target.insert(pair<string, string>("应用程序缓存02(谨慎删除)", user_path + "\\AppData\\Roaming"));
	target.insert(pair<string, string>("Windows 错误报告(推荐删除)", "C:\\ProgramData\\Microsoft\\Windows\\WER"));
	//target.insert(pair<string, string>("test1", "D:\\test1"));
	//target.insert(pair<string, string>("test2", "D:\\test2"));
	return target;
}
