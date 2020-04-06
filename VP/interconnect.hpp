#ifndef _INTERCONNECT_HPP_
#define _INTERCONNECT_HPP__

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

class interconnect :
	public sc_core::sc_module
{
public:
	interconnect(sc_core::sc_module_name);

	tlm_utils::simple_target_socket<interconnect> cpu_tsoc;
	tlm_utils::simple_initiator_socket<interconnect> inter_isoc;

protected:
	
	sc_dt::sc_uint<8> val;
	
	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void b_transport(pl_t&, sc_core::sc_time&);
	void msg(const pl_t&);
};

#endif
