// DigitalSys.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BasicElement.h"
#include "Component.h"
int main()
{

	//TEST 1 Basic element
#if 0
	Connector a;
	Connector b;
	Connector out1;
	Connector out2;
	Connector out3;
	And_Gate and1 = And_Gate(&a, &b, &out1);
	Or_Gate or1 = Or_Gate(&a, &b, &out2);
	Not_Gate not1 = Not_Gate(&a, &out3);
	Output o1 = Output(&out1);
	Output o2 = Output(&out2);
	std::string p1_name("out1");
	std::string p2_name("out2");
	std::string p3_name("not1");
	Probe Probe1 = Probe(p1_name, &out1);
	Probe Probe2 = Probe(p2_name, &out2);
	Probe Probe3 = Probe(p3_name, &out3);
	a.set_val(true);
	b.set_val(true);
	Main_Query_List.run();
#endif 

	//TEST 2 Serials of connector
#if 0
	Connector a;
	Connector b;
	Connector c;
	Not_Gate n1(&a, &b);
	Not_Gate n2(&b, &c);
	std::string na1 = "a";
	std::string na2 = "b";
	std::string na3 = "c";
	Probe p1(na1, &a);
	Probe p2(na2, &b);
	Probe p3(na3, &c);
	a.set_val(true);
	Main_Query_List.run();
#endif

	//Test 3 Component
#if 0
	Connector a;
	Connector b;
	Connector o1;
	Connector o2;
	And_Not_Gate andn(&a, &b, &o1);
	Xor_Gate xor (&a, &b, &o2);
	std::string n1("out");
	std::string n2("xor");
	Probe p1(n1, &o1);
	Probe p2(n2, &o2);
	a.set_val(true);
	b.set_val(false);
	Main_Query_List.run();
#endif

	//Test 4 set time
#if 0
	Connector a;
	Connector b;
	Connector o1;
	Xor_Gate xor (&a, &b, &o1);
	std::string n2("xor");
	Probe p1(n2, &o1);
	a.set_val(true);
	b.set_val(true);
	Main_Query_List.run(20);
	a.set_val(false);
	b.set_val(true);
	Main_Query_List.run(20);
	a.set_val(false);
	b.set_val(false);
	Main_Query_List.run(20);
	a.set_val(true);
	b.set_val(false);
	Main_Query_List.run(20);
#endif

	//Test 5 full-adder
#if 0
	Connector a;
	Connector b;
	Connector ci;
	Connector co;
	Connector s;
	Full_Adder fa(&a, &b, &s, &ci, &co);
	a.set_val(false);
	b.set_val(false);
	ci.set_val(true);
	std::string n1("ci");
	Probe p1(n1, &co);
	std::string n2("s");
	Probe p2(n2, &s);
	Main_Query_List.run();
#endif

	//Test 6 multiple and gate or gate
#if 0
	Connector a1;
	Connector a2;
	Connector a3;
	Connector a4;
	Connector out1;
	Connector out2;
	std::vector<Connector*> input = { &a1,&a2,&a3,&a4 };
	mAnd_Gate ma(&input, &out1);
	mOr_Gate mo(&input, &out2);
	std::string o1 = "mAnd";
	std::string o2 = "mOr";
	Probe p1(o1, &out1);
	Probe p2(o2, &out2);
	input[0]->set_val(true);
	input[1]->set_val(false);
	Main_Query_List.run(20);
	input[1]->set_val(true);
	input[2]->set_val(true);
	input[3]->set_val(true);
	Main_Query_List.run();
#endif

	//Test 7 multiple input and not gate
#if 0
	Connector a1;
	Connector a2;
	Connector a3;
	Connector a4;
	Connector out1;
	std::vector<Connector*> input = { &a1,&a2,&a3,&a4 };
	mAnd_Not_Gate ma(&input, &out1);
	std::string o1 = "mAndNot";
	Probe p1(o1, &out1);
	input[0]->set_val(true);
	input[1]->set_val(false);
	Main_Query_List.run(20);
	input[1]->set_val(true);
	input[2]->set_val(true);
	input[3]->set_val(true);
	Main_Query_List.run();
#endif

	//Test 8 encoder83
#if 0
	connector_list input(8);
	connector_list output(3);

	Encoder83 e1(&input, &output);
	std::string o1 = "o1";
	std::string o2 = "o2";
	std::string o3 = "o3";
	Probe p1(o1, output[0]);
	Probe p2(o2, output[1]);
	Probe p3(o3, output[2]);
	input[0]->set_val(true);
	Main_Query_List.run();
	std::cout << output[0]->get_val() << " " << output[1]->get_val() <<		" " << output[2]->get_val()<<"\n";
	input[0]->set_val(false);
	input[5]->set_val(true);
	Main_Query_List.run();
	std::cout << output[0]->get_val() << " " << output[1]->get_val() << " " << output[2]->get_val() << "\n";
#endif

	//Test 9 decoder38
#if 0
	connector_list input(3);
	connector_list output(8);
	Decoder38 e1(&input, &output);
	input[0]->set_val(true);
	input[1]->set_val(false);
	input[2]->set_val(false);
	Main_Query_List.run();
	for (int i = 0; i < 8; i++) {
		std::cout << output[i]->get_val() << " ";
	}
#endif

#if 1
	connector_list input1(8), input2(8), output(8);
	Ripple_Carry_Adder rca(8, &input1, &input2, &output);
	input1.set_value(7);
	input2.set_value(3);
	Main_Query_List.run();
	output.rshow_value();
#endif
	system("pause");
    return 0;
}

