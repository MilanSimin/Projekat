#ifndef _CPU_H_
#define _CPU_H_

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>




class cpu :
	public sc_core::sc_module
{
public:
	cpu(sc_core::sc_module_name);

	tlm_utils::simple_initiator_socket<cpu> int_isoc;
	tlm_utils::simple_initiator_socket<cpu> mem_isoc;

protected:
	
	
	int columns;
	int lines;
	void proces();
	
	void scan_infile(int* lines,int* columns);
	void addingZerosForEdges();
	void BlackAndWhite();
	void writeInFile();
	
	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void msg(const pl_t&);
};


#endif
