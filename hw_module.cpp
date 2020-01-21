#include <systemc.h>

using namespace std;

SC_MODULE(hw_module) {

	sc_in<sc_uint<8>> swi;
	sc_out<sc_uint<8>> led;
	sc_out<sc_uint<8>> hex[3];

	void process() {
		sc_uint<8> lastVal = 0;
		sc_uint<8> s = 0;

			s = swi.read();
			if (s == 1 && s != lastVal) { //task 1
				cout << "1 => Ogrzewanie z gory" << endl;
				led.write(1);
				hex[2].write('U');
				hex[1].write('P');
				hex[0].write('P');
			} else if ( (s == 0 || s == 2 || s == 4 || s == 8 || s == 16 || s == 32) && s != lastVal) {
				led.write(0);
				hex[2].write(' ');
				hex[1].write(' ');
				hex[0].write(' ');
			} else if (s != 0 && s != 1 && s != 2 && s != 4 && s != 8 && s != 16 && s != 32 && s != lastVal) {
				//led.write(0x80);
				hex[2].write('E');
				hex[1].write('R');
				hex[0].write('R');
			}

			lastVal = s;
	}

	SC_CTOR(hw_module) {
		SC_METHOD(process);
		sensitive << swi;
	}
};