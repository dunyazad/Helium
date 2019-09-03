#include <iostream>
using namespace std;

#include <Helium/Helium.h>
namespace AN = ArtificialNature;

void main()
{
	cout << "Hello Helium" << endl;

	auto he = HELIUM;
	auto window1 = he->CreateHeliumWindow(nullptr);
	//auto window2 = he->CreateHeliumWindow(nullptr);

	window1->CreateScene("window1");
	//window2->CreateScene("window2");

	he->Run();
}
