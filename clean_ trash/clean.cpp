#include<iostream>
#include<Thread>
#include<map>
#include"system_clean.h"
#include"MengFileManager.h"
using namespace std;
typedef unsigned int choies;

map<string, string> target_path;

//void testFunc(string path,MThreadPool& threadPool) {
//	if ((filesystem::status(path)).permissions() != filesystem::perms::none) {
//		cout << "程序拥有权限操作该文件" << endl;
//		this_thread::sleep_for(chrono::seconds(1));
//		threadPool.enterTask([&] {
//			for (auto& e : filesystem::directory_iterator(path)) {
//				if (e.exists() && e.is_regular_file()) {
//					cout << e.path() << endl;
//				}
//			}
//			});
//	}
//	this_thread::sleep_for(chrono::seconds(1));
//}

int main() {
	//target_path.insert(pair<string, string>("垃圾缓存文件(推荐删除)", "C:\\Users\\33953\\Downloads"));
	//target_path.insert(pair<string, string>("临时互联网文件01,浏览器缓存文件(推荐删除)", "C:\\ProgramData\\Microsoft\\Windows\\WER"));
	//target_path.insert(pair<string, string>("临时互联网文件02,浏览器缓存文件(推荐删除)", "C:\\Windows\\SoftwareDistribution\\Download"));
	//target_path.insert(pair<string, string>("Windows更新缓存(推荐删除)", "C:\\Users\\33953\\AppData\\Local\\Temp"));
	//target_path.insert(pair<string, string>("系统还原点(谨慎删除)", "C:\\Users\\33953\\AppData\\Local\\Microsoft\\Windows\\INetCache"));
	//target_path.insert(pair<string, string >("应用程序缓存01(你最好是手动删awa,有些删了能清理空间,有些删了会影响程序正常运行 ╰(艹皿艹 ) (谨慎删除) ", "C:\\Users\\33953\\AppData\\Local\\Microsoft\\Windows\\WebCache"));
	//target_path.insert(pair<string, string>("应用程序缓存02(谨慎删除)", "C:\\System Volume Information"));
	//target_path.insert(pair<string, string>("Windows 错误报告(推荐删除)", "C:\\Users\\33953\\AppData\\Local"));
	//target_path.insert(pair<string, string>("DirectX 着色器缓存(不推荐/谨慎删除)0.0", "C:\\Users\\33953\\AppData\\Roaming"));

	MThreadPool threadPool(12);
	MScreen meng(&threadPool);

	choies cs;
	while (true) {
		system("cls");
		meng.showMenu();
		cin >> cs;

		switch (cs) {
		case 1:
			cout << "将在1s后开始清理..." << '\n';
			this_thread::sleep_for(chrono::seconds(1));
			meng.startClean();
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