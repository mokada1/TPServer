#include "TPServer.h"
#include <locale.h>

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "KOREAN");
    TPServer::GetInstance().Play();
    return 0;
}