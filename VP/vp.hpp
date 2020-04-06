#ifndef _VP_HPP_
#define _VP_HPP_

#include <systemc>
#include "interconnect.hpp"
#include "convolution.hpp"
#include "CPU.hpp"
#include "memory.hpp"

class vp :
	sc_core::sc_module
{
public:
	vp(sc_core::sc_module_name);

protected:
	interconnect ic;
	conv con;
	cpu cp;
	memory mem;

};

#endif
