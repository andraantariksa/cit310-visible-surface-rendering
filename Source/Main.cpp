#include "Util/Logger.hpp"
#include "Core/App.hpp"
#include <cstdio>

#ifdef _DEBUG_BUILD
int main()
{
    freopen("log.txt", "w", stderr);

    App app;
    app.Run();
    return 0;
}
#else
#include <Windows.h>

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    freopen("log.txt", "w", stderr);

    App app;
    app.Run();
    return 0;
}
#endif
