#include "core/Application.h"

#include <Windows.h>

#undef main
int main(int argc, char** argv)
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// 创建应用程序实例
	Application app("逆界暗域", 1280, 720);

	// 初始化应用程序
	if(!app.initialize())
		return EXIT_FAILURE;

	// 运行应用程序
	app.run();

	return EXIT_SUCCESS;
}