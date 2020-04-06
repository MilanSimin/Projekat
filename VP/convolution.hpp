#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#define SIZE 512

const sc_dt::uint64 CONV_KERNEL = 1;
const sc_dt::uint64 CONV_LINES = 2;
const sc_dt::uint64 CONV_COLUMNS = 3;
const sc_dt::uint64 CONV_BEGIN = 4;

using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm;

class conv :
	public sc_core::sc_module
{
public:
	conv(sc_core::sc_module_name);

	tlm_utils::simple_target_socket<conv> int_tsoc;
	tlm_utils::simple_initiator_socket<conv> conv_isoc;


protected:
	
	int matrix [SIZE+2][SIZE+2]={{0}};		
	double kernel [3][3]={{0}};
	int lines,columns;
	
	void convolution();
	

	typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
	void b_transport(pl_t&, sc_core::sc_time&);
	void msg(const pl_t&);
};

#endif
