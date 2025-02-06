#include "MengFileManager.h"
namespace fs = filesystem;

MScreen::MScreen(MThreadPool* tP, map<string, string>& target_list)
{
	file_clean = new MFileClean(tP,target_list);
}

void MScreen::startClean()
{
	file_clean->scanFile();
}

void MScreen::showMenu()
{
	cout <<
		   "\t \t  ██████╗ █████╗ ███╗   ██╗███╗   ███╗███████╗███╗   ██╗ ██████╗" << '\n'
		<< "\t \t ██╔════╝██╔══██╗████╗  ██║████╗ ████║██╔════╝████╗  ██║██╔════╝" << '\n'
		<< "\t \t ██║     ███████║██╔██╗ ██║██╔████╔██║█████╗  ██╔██╗ ██║██║  ███╗" << '\n'
		<< "\t \t ██║     ██╔══██║██║╚██╗██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║" << '\n'
		<< "\t \t ╚██████╗██║  ██║██║ ╚████║██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝" << '\n'
		<< "\t \t ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝" << '\n' << '\n' << '\n'
		<< "\t \t *欢迎使用CanMeng西盘清理工具:( 这里为你提供西盘清理服↗务↘ awa *" << '\n' //*64
		<< "\t \t *****************************************************************" << '\n'
		<< "\t \t                          1.清理C盘  0.0                        " << '\n'
		<< "\t \t                          2.程序详情(⊙_⊙)？                     " << '\n';
}

void MScreen::showDetails()
{
	cout << "this is shit。。。" << '\n' << "软件有概率卡住0.0  能不能成功扫描全凭运气。" << '\n';
}
