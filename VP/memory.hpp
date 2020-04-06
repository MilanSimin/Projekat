#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>


class memory :
	public sc_core::sc_module
{
	public:
		memory(sc_core::sc_module_name);
		tlm_utils::simple_target_socket<memory> cpu_tsoc;
		tlm_utils::simple_target_socket<memory> conv_tsoc;

		

	protected:

		typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t; 
		void b_transport (pl_t&, sc_core::sc_time&);
		unsigned int transport_dbg(pl_t&);
	       	void msg(const pl_t&);
			
};

#endif
