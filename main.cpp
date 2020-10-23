#include "server.h"
int main()
{
    Server s(3456);
    s.init();
    s.start();
    return 0;
}
