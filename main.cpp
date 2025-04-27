// pygmalion.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "main.h"
#include "libCommon.h"
#include "libBitboard.h"
#include "libMovegen.h"
#include "libEngine.h"
#include <iostream>
using namespace std;

int main()
{
	Engine engine(std::cin, std::cout);
	engine.Start();
	engine.WaitForStop();
	return 0;
}
