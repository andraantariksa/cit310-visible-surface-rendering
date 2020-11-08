#include "Util/Logger.hpp"
#include "Core/App.hpp"

#ifdef _DEBUG_BUILD
int main()
{
    App app;
    app.Run();
    return 0;
}
#else

#include <Windows.h>

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    App app;
    app.Run();
    return 0;
}
#endif
