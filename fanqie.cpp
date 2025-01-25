// MyPlugin.cpp
#include "fanqie.h"

extern "C"
{
    IPlugin *createPlugin()
    {
        return new Fanqie();
    }
}