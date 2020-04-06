#include "interconnect.hpp"
#include "vp_addr.hpp"
#include <string>

using namespace std;
using namespace tlm;
using namespace sc_core;
using namespace sc_dt;

interconnect::interconnect(sc_module_name name) :
	sc_module(name)
{
	cpu_tsoc.register_b_transport(this, &interconnect::b_transport);
}

void interconnect::b_transport(pl_t& pl, sc_core::sc_time& offset)
{
	uint64 addr = pl.get_address();
	uint64 taddr;
	offset += sc_time(2, SC_NS);
	
	
	if(addr >= VP_ADDR_CONV && addr <= VP_ADDR_CONV_BEGIN){
		taddr= addr & 0x0000000F;
		pl.set_address(taddr);
		inter_isoc->b_transport(pl, offset);
		
	}

		pl.set_address(addr);

	
}

void interconnect::msg(const pl_t& pl)
{

}
