#pragma once
#include "BasicElement.h"

class And_Not_Gate {
	cptr tmp;
	And_Gate and;
	Not_Gate not;
public:
	And_Not_Gate() = default;
	And_Not_Gate(cptr a,cptr b,cptr out)
		:and (a, b, tmp), not(tmp, out) {}

	void set_pin(cptr a, cptr b, cptr out) {
		and.set_pin(a, b, tmp);
		not.set_pin(tmp, out);
	}
};

/* multiple input and not gate */

class mAnd_Not_Gate {
	cptr tmp;
	mAnd_Gate mand;
	Not_Gate not;
public:
	mAnd_Not_Gate() = default;
	mAnd_Not_Gate(connector_list* input, cptr out)
		:mand(input, tmp), not(tmp, out) {}

	void set_pin(connector_list* input, cptr out) {
		mand.set_pin(input, tmp);
		not.set_pin(tmp, out);
	}
};

class Or_Not_Gate {
	cptr tmp;
	Or_Gate or;
	Not_Gate not;
public:
	Or_Not_Gate() = default;
	Or_Not_Gate(cptr a, cptr b, cptr out)
		:or (a, b, tmp), not(tmp, out) {}

	void set_pin(cptr a, cptr b, cptr out) {
		or .set_pin(a, b, tmp);
		not.set_pin(tmp, out);
	}
};


/* multiple input or not gate */

class mOr_Not_Gate {
	cptr tmp;
	mOr_Gate mor;
	Not_Gate not;
public:
	mOr_Not_Gate() = default;
	mOr_Not_Gate(connector_list* input, cptr out)
		:mor(input, tmp), not(tmp, out) {}

	void set_pin(connector_list* input, cptr out) {
		mor.set_pin(input, tmp);
		not.set_pin(tmp, out);
	}
};

class Xor_Gate {
	cptr tmp1;
	cptr tmp2;
	And_Gate and;
	And_Not_Gate and_not;
	Or_Gate or;
public:
	Xor_Gate() = default;
	Xor_Gate(cptr a, cptr b, cptr out) 
		:and_not(a, b, tmp1), or (a, b, tmp2), and (tmp1, tmp2, out) {}

	void set_pin(cptr a, cptr b, cptr out) {
		and_not.set_pin(a, b, tmp1);
		or .set_pin(a, b, tmp2);
		and.set_pin(tmp1, tmp2, out);
	}
};

class Half_Adder {
	Xor_Gate xor;
	And_Gate and;
public:
	Half_Adder() = default;
	Half_Adder(cptr a, cptr b, cptr o, cptr c)
		:xor (a, b, o), and (a, b, c) {}

	void set_pin(cptr a, cptr b, cptr o, cptr c) {
		xor.set_pin(a, b, o);
		and.set_pin(a, b, c);
	}
};

class Full_Adder {
	cptr tmp_out;
	cptr tmp_c1;
	cptr tmp_c2;
	Half_Adder ha1;
	Half_Adder ha2;
	Or_Gate or;
public:
	Full_Adder() = default;
	Full_Adder(cptr a, cptr b, cptr o, cptr ci, cptr co)
		:ha1(a, b, tmp_out, tmp_c1), ha2(ci, tmp_out, o, tmp_c2),
		or(tmp_c1, tmp_c2, co) {};

	void set_pin(cptr a, cptr b, cptr o, cptr ci, cptr co) {
		ha1.set_pin(a, b, tmp_out, tmp_c1);
		ha2.set_pin(ci, tmp_out, o, tmp_c2);
		or.set_pin(tmp_c1, tmp_c2, co);
	}
};

/* implement of 8 to 3 digital encoder */

class Encoder83 {
	mOr_Gate mor1;
	mOr_Gate mor2;
	mOr_Gate mor3;
public:
	Encoder83(connector_list* input, connector_list* output){
		if (input->size() != 8) {
			std::cerr << "8 to 3 Encoder: input connectors' number is not 8!";
		}else if (output->size() != 3) {
			std::cerr << "8 to 3 Encoder: output connectors' number is not 3!";
		}else {
			connector_list i1{ (*input)[1],(*input)[3],(*input)[5],(*input)[7] };
			connector_list i2{ (*input)[2],(*input)[3],(*input)[6],(*input)[7] };
			connector_list i3{ (*input)[4],(*input)[5],(*input)[6],(*input)[7] };
			mor1.set_pin(&i1, (*output)[0]);
			mor2.set_pin(&i2, (*output)[1]);
			mor3.set_pin(&i3, (*output)[2]);
		}
	}
};

class Decoder38 {
	connector_list n;
	Not_Gate not[3];
	mAnd_Gate mand[8];
public:
	Decoder38(connector_list* input, connector_list* output) :n(3) {
		if (input->size() != 3) {
			std::cerr << "3 to 8 Encoder: input connectors' number is not 3!";
		}
		else if (output->size() != 8) {
			std::cerr << "3 to 8 Encoder: output connectors' number is not 8!";
		}
		else {
			for (int i = 0; i < 3; i++) {
				not[i].set_pin((*input)[i], n[i]);
			}
			//用位运算直接算出为 1 的位用高电平，为 0 的位取反
			for (int i = 0; i < 8; i++) {
				int tmp1 = i,tmp2 = 4;
				connector_list tmp_input;
				for (int j = 0; j < 3; j++) {
					//如果第 j 位为 1
					if (tmp1&tmp2) {
						tmp_input.push_back((*input)[j]);
					}else {
						tmp_input.push_back(n[j]);
					}
					tmp2 >>= 1;
				}
				mand[i].set_pin(&tmp_input, (*output)[i]);
			}
		}
	}
};

class Ripple_Carry_Adder {
	int digits;
	connector_list ctmp;
	std::vector<Full_Adder> adder_list;
	Half_Adder begin_adder;
public:
	Ripple_Carry_Adder(int d, connector_list* input1, connector_list* input2, connector_list* output) 
		:digits(d), adder_list(digits - 1),ctmp(digits) {
		int isize1 = input1->size(),
			isize2 = input2->size(),
			osize = output->size();
		if (isize1 < digits) {
			std::cout << "Ripple Carry Adder: input1 size is too small. (It will be filled)";
			for (int i = 0; i < digits - isize1; i++) {
				cptr tmp(new Connector());
				input1->push_back(tmp);
			}
		}
		if (osize < digits) {
			std::cout << "Ripple Carry Adder: input2 size is too small. (It will be filled)";
			for (int i = 0; i < digits - isize2; i++) {
				cptr tmp(new Connector());
				input2->push_back(tmp);
			}
		}
		if (osize < digits) {
			std::cout << "Ripple Carry Adder: output size is too small. (It will be filled)";
			for (int i = 0; i < digits - osize; i++) {
				cptr tmp(new Connector());
				output->push_back(tmp);
			}
		}
		begin_adder.set_pin((*input1)[0], (*input2)[0], (*output)[0], ctmp[0]);
		for (int i = 1; i <= digits - 1; i++) {
			adder_list[i-1].set_pin((*input1)[i], (*input2)[i], (*output)[i], ctmp[i - 1], ctmp[i]);
		}
	}

	int get_digits() { return digits; }
};

class Multiplexer4 {
	connector_list n;
	connector_list ctmp;
	mAnd_Gate and[4];
	Not_Gate not[2];
	mOr_Gate or ;
public:
	Multiplexer4() = default;
	Multiplexer4(connector_list* inputI, connector_list* inputAB, cptr output)
		:n(2), ctmp(4) {
		not[0].set_pin((*inputAB)[0],n[0]);
		not[1].set_pin((*inputAB)[1],n[1]);
		connector_list tmp[4] = 
		{ { (*inputI)[0],n[0],n[1] },
		{ (*inputI)[1],(*inputAB)[0],n[1] },
		{ (*inputI)[2],n[0],(*inputAB)[1] },
		{ (*inputI)[3],(*inputAB)[0],(*inputAB)[1] } };
		for (int i = 0; i < 4; i++) {
			and[i].set_pin(&tmp[i], ctmp[i]);
		}
		or .set_pin(&ctmp, output);
	}
};

class Demultiplexer {

};