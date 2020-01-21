#include <systemc.h>
#include <conio.h>
#include "channel.h"


using namespace std;

SC_MODULE(cpu1) {
	sc_inout<sc_uint<8>> switches;
	sc_out<sc_uint<8>> hex[6];
	sc_out<sc_uint<8>> leds;
	sc_port<write_fifo> out;
	sc_buffer<sc_uint<8>> MessageBox;
	Channel* ch;


	bool er = false;
	int modif = 48; 


	void task1() {

		int f = 0;
		int lastVal = 0;

		while (1) {
			wait(500, SC_NS);
			if (_kbhit()) {
				f = _getch();
				//cout << f << endl; //ESC => 27 space => 32
				
				if (f >= 49 && f <= 54) {
					f = toHex(f - modif);


					sc_uint<8> s = switches.read();
					sc_uint<8> cBit = 0x20;

					while (cBit != 0x0) {
						if (cBit & f) { //
							if (cBit & s) s = s - cBit; //switch was turned on
							else       s = s ^ cBit;  //swicth was off
						}
						cBit = cBit >> 1;
					}
					switches.write(s);
				} else if(f == 32) {
					showBits(leds.read(), "LED", 0x40);
					showBits(switches.read(), "SWI", 0x20);
					showHex();
				} else if (f == 27) {
					sc_stop();
				}
			}

			sc_uint<8> s = switches.read();

			if (s != 0 && s != 1 && s != 2 && s != 4 && s != 8 && s != 16 && s != 32 && s != lastVal) { // write error
				cout << "E => Error!" << endl;
				leds.write(0x40); //turn on led #7 (error led)
				hex[2].write('E');
				hex[1].write('R');
				hex[0].write('R');
			} else if ( (s == 0 || s == 1) && s != lastVal) { //clear
				cout << " " << endl;
				leds.write(0);
				hex[2].write(' ');
				hex[1].write(' ');
				hex[0].write(' ');
			}

			//task 1 is implemented in hw_module

			out->write(s); //FIFO CHANNEL (cpu1 <-> cpu2)
			MessageBox.write(s); //MESSAGE BOX ( cpu1 <-> cpu1 )
			lastVal = s;
			wait(1, SC_SEC);
		}
	}

	void task2() {
		sc_uint<8> lastVal = 0;
		while (1) {
			sc_uint<8> s = MessageBox.read();

			if (s == 2 && s != lastVal) {
				cout << "2 => Ogrzewanie z dolu" << endl;
				leds.write(2);
				hex[2].write('B');
				hex[1].write('O');
				hex[0].write('T');
			}
			lastVal = s;
			wait(1, SC_SEC);
		}
	}

	void task3() {
		sc_uint<8> lastVal = 0;
		while (1) {
			sc_uint<8> s = MessageBox.read();

			if (s == 4 && s != lastVal) {
				cout << "3 => Ogrzewanie z lewej" << endl;
				leds.write(4);
				hex[2].write('L');
				hex[1].write('E');
				hex[0].write('F');
			}
			lastVal = s;
			wait(1, SC_SEC);
		}
	}

	void task4() {
		sc_uint<8> lastVal = 0;
		while (1) {
			sc_uint<8> s = MessageBox.read();

			if (s == 8 && s != lastVal) {
				cout << "4 => Ogrzewanie z prawej" << endl;
				leds.write(8);
				hex[2].write('R');
				hex[1].write('I');
				hex[0].write('G');
			}
			lastVal = s;
			wait(1, SC_SEC);
		}
	}

	void task5() {
		sc_uint<8> lastVal = 0;
		while (1) {
			sc_uint<8> s = MessageBox.read();

			if (s == 16 && s != lastVal) {
				cout << "5 => Ogrzewanie ze wszystkich stron" << endl;
				leds.write(16);
				hex[2].write('A');
				hex[1].write('L');
				hex[0].write('L');
			}
			lastVal = s;
			wait(1, SC_SEC);
		}
	}

	void showBits(sc_uint<8> state, string title, sc_uint<8> cBit) {
		cout << title << ":\t";
		while (cBit != 0x0) {
			cout << (state & cBit ? "1" : "0");
			if (cBit == 0x1) cout << endl; else cout << " | ";
			cBit = cBit >> 1;
		}

	}

	void showHex() {

		sc_uint<8> state[6];
		for (int i = 0; i < 6; i++)
			state[i] = hex[i].read();

		cout << "HEX:\t";
		for (int i = 5; i >= 0; i--) {
			//cout << state[i];
			cout << (char)state[i];

			if (i == 0) cout << endl; else cout << " | ";
		}
	}

	sc_uint<8> toHex(int value) {

		switch (value) {
		case 0:
		case 1:
		case 2: return value;
		case 3: return 0x4;
		case 4: return 0x8;
		case 5: return 0x10;
		case 6: return 0x20;
		default: return 0;
		}
	}

	SC_CTOR(cpu1) {
		SC_THREAD(task1);
		SC_THREAD(task2);
		SC_THREAD(task3);
		SC_THREAD(task4);
		SC_THREAD(task5);
	}
};