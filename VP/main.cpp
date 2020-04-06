#include <systemc>
#include "vp.hpp"

using namespace sc_core;
using namespace std;

int sc_main(int argc, char* argv[])
{
	vp uut("uut");
	//cout<<"proba"<<endl;
	sc_start(10, sc_core::SC_SEC);

    return 0;
}
