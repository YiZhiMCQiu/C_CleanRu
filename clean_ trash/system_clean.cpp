#include "system_clean.h"
using namespace std;
namespace fs = filesystem;

MFileClean::MFileClean()
{
}

MFileClean::MFileClean(MThreadPool* tP, map<string, string>& tempSpList)
{
    threadPool = tP;

    target_path = tempSpList;

}

void MFileClean::coutGetsFileList()
{
    cout << "**********************************************************" << '\n'
        << "**********************************************************" << '\n'
        << "**********************************************************" << '\n'
        << "**********************************************************" << '\n'
        << "**********************************************************" << '\n'
        << "**********************************************************" << '\n'
        << "**********************************************************" << '\n';

function<void()> printTitleAFile = [this] {
    for (auto pa : getsFileList) {
        cout <<
            "***************************************************************"
            << '\n' <<
            pa.first
            << "\n"
            << "***************************************************************"
            << "\n";
        for (auto path : pa.second) {
            unsigned index = path.rfind("\\");
            cout << path.substr(index + 1, path.length()) << '\n';
        }
    }
    };

unsigned choies, actualFileNum;
choies = 3; actualFileNum = 0;
for (auto e : getsFileList) {
    actualFileNum += e.second.size();
}
cout << "扫描完毕,本次扫描共计:" << allFindNum << "个文件\t" << "实际获取到文件:" << actualFileNum << '\n' << "-----------------------------------------------------------" << '\n'
<< "按1查看具体文件,按2直接删除" << endl;
cin >> choies;
system("cls");
map<int, pair<string, set<string>>> tempContainer;
unsigned index = 0;
for (auto& tempPair : getsFileList) {
    tempContainer.insert(pair<int, pair<string, set<string>>>(index, tempPair));
    index++;
}

function<int(set<string>)> deleteFile = [](set<string> target) {
    system("cls");
    unsigned fileSize = 0;
    for (auto e : target) {
        if (fs::exists(e) && (fs::status(e).permissions() & fs::perms::owner_read | fs::perms::owner_exec | fs::perms::owner_write) != fs::perms::none) {
            fileSize += fs::file_size(e);
            fs::remove(e);
        }
    }
    return fileSize;
    };

function<string(int oldByte)> convertByte = [](int oldByte) {
    string size;
    if (oldByte >= 1024) {
        if (oldByte >= 1048576) {
            if (oldByte >= 1073741824) {
                size = to_string(oldByte / 1073741824) + "GB";
                return size;
            }
            size = to_string(oldByte / 1048576) + "MB";
            return size;
        }
        size = to_string(oldByte / 1024) + "kb";
        return size;
    }
    };

switch (choies) {
case 1:
    printTitleAFile();
    cout << "以上为具体目录,可单独选择进行删除(可能有点长,可以翻一下):" << '\n';
        for (auto e : getsFileList) {
            cout << "-----------------------------" <<
                '\n' << e.first << ":" << e.second.size() << "个" << '\n';
        }
    cout << "-----------------------------" << '\n' << '\n';

    while (1) {
    //AGAIN:
        if (tempContainer.empty()) {
            cout << "电脑已经很干净了,按任意键返回..." << '\n';
            system("pause");
            return;
        }
        for (auto teOb : tempContainer) {
            try {
                unsigned fil = 0;
                for (auto e : teOb.second.second) {
                    fil += fs::file_size(e);
                }
                string size = convertByte(fil);
                cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - " << '\n' << teOb.first << "." << teOb.second.first << '\t' << size << '\n';

            }
            catch (const fs::filesystem_error& e) {
                cerr << "遍历文件大小错误,建议重启程序再试..." << endl;
                system("pause");
                return;
            }
            
        }
        cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - " << '\n';
        cout << '\n' << "请选择相应的数字进行删除" << '\n';
        int choose = 0;
        cin >> choose;

        
        map<int, pair<string, set<string>>>::iterator it = tempContainer.find(choose);
        
        if (it == tempContainer.end()) {
            cout << "数字无效,请重新输入..." << endl;
            system("pause");
            system("cls");
            continue;
        }

        for (auto e : (*it).second.second) {
            cout << e << endl;
        }

        try {
            if (it != tempContainer.end()) {
                set<string> tar = (*it).second.second;
                int tePa = deleteFile(tar);
                tempContainer.erase(it);
                cout<< "删除成功,共计:" << convertByte(tePa) << '\n';
            }
        }
        catch (const fs::filesystem_error& e) {
            cerr << "删除文件时发生错误,请重试..." << '\n';
            system("pause");
            return;
        }
    }
case 2:
    cout << "删除中,请稍后..." << '\n';
    
    try {
        for (auto e : tempContainer) {
            int tePa = deleteFile(e.second.second);
            cout << "全部删除成功！共计:" << convertByte(tePa) << '\n';
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "删除文件时发生错误..." << endl;
    }

    tempContainer.clear();
    system("pause");
    return;
}
}

void MFileClean::scanFile()
{
    cout << "start" << endl;
    try {
        if (!(threadPool != nullptr)) {
            throw runtime_error("线程池未能正确初始化");
        }
    }
    catch (runtime_error& e){
        cout << "error" << endl;
    }

        for (auto path : target_path) {
            threadPool->enterTask([path, this] {

                try {
                    function<pair<int, set<string>>(string path)> reDir = [&reDir](string path) -> pair<int, set<string>>{
                        unsigned findNum = 0;
                        set<string> fileList_1;
                        for (auto& target : fs::directory_iterator(path)) {
                            if (target.is_directory()) {
                                auto fileList_2 = reDir(target.path().string());
                                fileList_1.insert(fileList_2.second.begin(), fileList_2.second.end());
                                findNum += fileList_2.first;
                            }
                            else {
                                if (fs::is_regular_file(target)) {
                                    fileList_1.insert(target.path().string());
                                    findNum++;
                                    cout << target.path() << endl;
                                }
                            }
                        }
                        pair<int, set<string>> findData(findNum, fileList_1);
                        return findData;
                        };

                    set<string> fileList;
                    for (auto& target : fs::directory_iterator(path.second)) {
                        if (target.is_directory()) {
                            int index = target.path().string().rfind("\\");
                            string dirName = target.path().string().substr(index + 1);
                            if (dirName.compare("Microsoft")) {
                                auto fileList_3 = reDir(target.path().string());
                                fileList.insert(fileList_3.second.begin(), fileList_3.second.end());
                                allFindNum+=fileList_3.first;
                            }
                        }
                        else {
                            fileList.insert(target.path().string());
                            allFindNum++;
                            cout << target.path() << endl;
                        }
                    }
                    getsFileList.insert(pair<string,set<string>>(path.first,fileList));
                }
                catch (const fs::filesystem_error& e) {
                    cerr << "访问文件出错..." << endl;
                    allFindNum++;
            }
                });
        }
        cout << "test:成功" << endl;
        if (getsFileList.empty()) {
            cerr << "未知错误,未能正确地获取获取到目标文件夹内文件,请尝试重启软件再试！" << '\n';
            system("pause");
            return;
        }

        bool isComplete = threadPool->waitTaskComplete();
        if (isComplete) {
            coutGetsFileList();
        } 
}

string MFileClean::getUserDir()
{
    for (auto& e : filesystem::directory_iterator("C:\\Users")) {
        if (filesystem::is_directory(e.path().string() + "\\Music")) {
            return e.path().string();
        }
    }
}

