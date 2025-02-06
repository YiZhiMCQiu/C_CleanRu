#include "system_clean.h"
using namespace std;
namespace fs = filesystem;

MFileClean::MFileClean()
{
}

MFileClean::MFileClean(MThreadPool* tP)
{
    threadPool = tP;

    user = getUserDir();
    path = user->path().string();
    cout << path << '\n';

    if (filesystem::is_directory(path + "\\Downloads")) {
        target_path.insert(pair<string,string>("C盘下载文件夹(谨慎删除)",path+"\\Downloads"));
    }
    target_path.insert(pair<string, string>("垃圾缓存文件(推荐删除)", path + "\\AppData\\Local\\Temp"));
    target_path.insert(pair<string, string>("预取文件,加快程序启动速度缓存文件(谨慎删除)", "C:\\Windows\\prefetch"));
    target_path.insert(pair<string, string>("临时互联网文件01,浏览器缓存文件(推荐删除)", user->path().string() + "\\AppData\\Local\\Microsoft\\Windows\\INetCache"));
    target_path.insert(pair<string, string>("临时互联网文件02,浏览器缓存文件(推荐删除)", user->path().string() +  "\\AppData\\Local\\Microsoft\\Windows\\WebCache"));
    target_path.insert(pair<string, string>("Windows更新缓存(推荐删除)", "C:\\Windows\\SoftwareDistribution\\Download"));
    //target_path.insert(pair<string, string>("系统还原点(谨慎删除)", "C:\\System Volume Information"));
    target_path.insert(pair<string, string > ("应用程序缓存01(你最好是手动删awa,有些删了能清理空间,有些删了会影响程序正常运行 ╰(艹皿艹 ) (谨慎删除) ", user->path().string() + "\\AppData\\Local"));
    target_path.insert(pair<string, string>("应用程序缓存02(谨慎删除)",user->path().string() + "\\AppData\\Roaming"));
    target_path.insert(pair<string, string>("Windows 错误报告(推荐删除)", "C:\\ProgramData\\Microsoft\\Windows\\WER"));
    target_path.insert(pair<string, string>("DirectX 着色器缓存(不推荐/谨慎删除)0.0", "C:\\Windows\\System32\\DriverStore\\FileRepository\\dxcache"));

    for (auto e : target_path) {
        cout << e.first << endl;
    }
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
    cout << "扫描完毕,本次扫描共计:" << allFindNum << "个文件\t" << "实际获取到文件:" << actualFileNum << '\n'
        << "按1查看具体文件,按2直接删除" << endl;
    cin >> choies;
    system("cls");
    map<int, pair<string, set<string>>> tempContainer;
    unsigned index = 0;
    for (auto& tempPair : getsFileList) {
        tempContainer.insert(pair<int, pair<string, set<string>>>(index, tempPair));
        index++;
    }

    switch (choies) {
    case 1:
        printTitleAFile();
        cout << "以上为具体目录,可单独选择进行删除(可能有点长,可以翻一下):" << '\n';
        for (auto e : getsFileList) {
            cout << "-----------------------------" <<
                '\n' << e.first << ":" << e.second.size() << "个" << '\n'
                << "-----------------------------" << '\n' << '\n';
        }
        
        while (1) {
            AGAIN:
            for (auto& teOb: tempContainer) {
                cout << teOb.first << "." << teOb.second.first << '\n';
            }
            cout <<'\n' << "请选择相应的数字进行删除" << '\n';
            int choose = 0;
            cin >> choose;

            map<int, pair<string, set<string>>>::iterator it = tempContainer.find(choose);
            if (it != tempContainer.end()) {
                for (auto& ob : (*it).second.second) {
                    try {
                        tempContainer.erase(it);
                        if (fs::remove(ob)) {
                            cout << "文件删除成功！" << '\n';

                        }else{
                            cout << "文件删除失败，可能不存在！" << '\n';}
                        goto AGAIN;
                    }catch(const fs::filesystem_error&e){
                        cerr << "删除文件时发生错误，可能没有权限删除此文件..." << '\n';
                    }
                }
            }
            else {
                cout << "请输入一个有效数字！" << '\n';
                this_thread::sleep_for(chrono::seconds(2));
                continue;
            }
        }
    case 2:
        cout << "删除中,请稍后..." << '\n';
        int unAchieveDelete = 0;
        for (auto e : tempContainer) {
            for (auto filePath : e.second.second) {
                try {
                    if (!fs::remove(filePath)) {
                        unAchieveDelete++;
                    }
                }catch(const fs::filesystem_error& e){
                    unAchieveDelete++;
                }
            }
        }
        cout << "文件已全部删除完毕|成功删除文件" 
            << tempContainer.size() - unAchieveDelete 
            << "个|文件删除失败" 
            << unAchieveDelete << "个|" << '\n';
    }
}

void MFileClean::scanFile()
{
    cout << "start" << endl;
    if (!(threadPool != nullptr)) {
        cerr << "未知错误,线程池未能正确初始化..." << '\n';
     } 

        for (auto path : target_path) {
                threadPool->enterTask([path, this] {
                    try{
                        function<pair<set<string>, int>(const string&)> findFile = [&findFile](const string& path) {
                            int allFindNum = 0;
                            if (fs::exists(path)) {
                                if ((fs::status(path).permissions() & fs::perms::owner_read) != fs::perms::none) {
                                    set<string> list;
                                    for (auto& e : fs::directory_iterator(path)) {
                                        if (e.is_directory()) {
                                            pair<set<string>,int> files = findFile(e.path().string());
                                            allFindNum += files.second;
                                            for (auto e : files.first) {
                                                list.insert(e);
                                            }
                                        }
                                        else if (e.is_regular_file()) {
                                            list.insert(e.path().string());
                                            allFindNum++;
                                        }
                                    }
                                    return pair<set<string>, int>(list,allFindNum);
                                }
                            }
                            };

                        set<string> reFile;
                        if (fs::exists(path.second) && fs::status(path.second).permissions() != fs::perms::owner_read) {
                            for (auto& target : fs::directory_iterator(path.second)) {
                                if (target.is_directory()) {
                                    pair<set<string>,int> list = findFile(target.path().string());
                                    for (auto e : list.first) {
                                        cout << e << endl;
                                        reFile.insert(e);
                                    }
                                    allFindNum += list.second;
                                }
                                else if (target.exists() && target.is_regular_file()) {
                                    cout << target.path() << endl;
                                    reFile.insert(target.path().string());
                                }
                                else {
                                    continue;
                                }
                            }
                        }
                        getsFileList.insert(pair<string, set<string>>(path.first, reFile));
                    }
                    catch (fs::filesystem_error& e) {
                        if (e.code() == errc::permission_denied) {
                            cerr << "无权限访问该文件" << '\n';
                        }
                    } 
                    });
                
        }
        bool isComplete = threadPool->waitTaskComplete();
        if (isComplete) {
            coutGetsFileList();
        }

    cout << "*************************************************************" << endl;
    

    if (getsFileList.empty()) {
        cerr << "未知错误,未能完全/正确地获取获取到目标文件夹内文件..." << '\n';
    }
    this_thread::sleep_for(chrono::seconds(1));   
    cout << "test01 :正确运行" << '\n';
}

fileDirEntry* MFileClean::getUserDir() {
    for (auto& e : filesystem::directory_iterator("C:\\Users")) {
        if (filesystem::is_directory(e.path().string() + "\\Music")) {
            path = "C:\\Users" + e.path().string();
            return new filesystem::directory_entry(e);
        }
    }
}