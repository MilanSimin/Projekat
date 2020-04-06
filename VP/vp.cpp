#include "vp.hpp"
#include <iostream>

using namespace sc_core;

vp::vp(sc_module_name name) :
	sc_module(name),
	cp("cpu"),
	ic("interconnect"),
	con("conv"),
	mem("memory")
{
	cp.mem_isoc.bind(mem.cpu_tsoc);
	cp.int_isoc.bind(ic.cpu_tsoc);
	ic.inter_isoc.bind(con.int_tsoc);
	con.conv_isoc.bind(mem.conv_tsoc);

	SC_REPORT_INFO("VP", "Platform is constructed");
}
