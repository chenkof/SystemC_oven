#include "systemc.h"
#include "channel.h"
#include "CPU_1.cpp"
#include "CPU_2.cpp"
#include "hw_module.cpp"

const int hexSegments = 6;
Channel* ch;

int sc_main(int argc, char* argv[]) {

	sc_signal<sc_uint<8>, SC_MANY_WRITERS> leds;
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> switches;
	sc_signal<sc_uint<8>, SC_MANY_WRITERS> hex[hexSegments];

	//queue q("queue");
	ch = new Channel("fifo");
	//fifo

	cpu1 cpu_1("cpu1");
	cpu_1.out(*ch);
	cpu_1.leds(leds);
	cpu_1.switches(switches);

	for (int i = 0; i < hexSegments; i++)
		cpu_1.hex[i](hex[i]);

	cpu2 cpu_2("cpu2");
	cpu_2.in(*ch);
	cpu_2.leds(leds);

	for (int i = 0; i < 3; i++)
		cpu_2.hex[i](hex[i]);


	hw_module module("module");
	module.led(leds);
	module.swi(switches);

	for (int i = 3; i < 6; i++) //hw module uses different hexes
		module.hex[i-3](hex[i]);

	sc_start();

	cout << endl << endl;
	return 0;
}