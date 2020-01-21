#ifndef CHANNEL_FIFO
#define CHANNEL_FIFO
#include "systemc.h"

class read_fifo : virtual public sc_interface {
public:
	virtual bool read(sc_uint<8> &v) = 0;
};

class write_fifo : virtual public sc_interface {
public:
	virtual bool write(sc_uint<8> v) = 0;
};


class Channel: public sc_channel, public read_fifo, public write_fifo {
public:
	sc_fifo<sc_uint<8>> FIFO;

	Channel(sc_module_name n) : sc_channel(n) {
		sc_fifo<sc_uint<8>> FIFO(2);
	}

	bool read(sc_uint<8> &v) {
		return FIFO.nb_read(v);
	}

	bool write(sc_uint<8> v) {
		return FIFO.nb_write(v);
	}
};

#endif 