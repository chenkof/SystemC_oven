#include <systemc.h>
#include "channel.h"

using namespace std;

SC_MODULE(cpu2)
{
	sc_out<sc_uint<8>> hex[3];
	sc_out<sc_uint<8>> leds;
	sc_port<read_fifo> in;
	Channel* ch;

	void task1() {
		sc_uint<8> lastVal = 0;
		sc_uint<8> s;

		while (1) {

			
			
			if (in->read(s)) {
				if (s == 32 && s != lastVal) {
					cout << "6 => Termoobieg" << endl;
					hex[2].write('T');
					hex[1].write('E');
					hex[0].write('R');
				}
				lastVal = s;
			}

			wait(1, SC_SEC);
		}
	}

	SC_CTOR(cpu2) {
		SC_THREAD(task1);
	}
};