#include "Profiler.h"

// Platform booleans
bool windows;
bool macOs;
bool linux;
bool unix;
bool unknown;

bool getOS(int n)
{
    switch (n)
    {
        case 0:
            windows = true;
            return windows;
        case 1:
            macOs = true;
            return macOs;
        case 2:
            linux = true;
            return linux;
        case 3:
            unix = true;
            return unix;
        case 4:
            unknown = true;
            return unknown;
    }
}

void showProfiler(bool* p_open)
{

}