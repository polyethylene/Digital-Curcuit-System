#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <deque>
#include <assert.h>
#include <memory>

using std::shared_ptr;

/*
class Signal {
	typedef std::vector<std::pair<bool, double>> signal_string;
	signal_string content;	
	~Signal() = default;
public:
	Signal() = default;
	Signal(signal_string& con) :content(con) {};
};
*/

class Element {
	friend class Query_List;
	friend void call(std::vector<Element*> ele);
	virtual void work() {};
protected:
	double delay = 0;
public: 
	double get_delay() { return delay; }
};

class Connector {
	bool val;
	std::vector<Element*> to;
public:
	Connector() :val(false) {};
	Connector(bool v) :val(v) {};
	void set_val(bool v) {
		if (v!= val) {
			val = v;
			call(to);
		}
	}
	void add_to(Element* t) { to.push_back(t); }
	bool get_val() { return val; }
};

//using cptr = shared_ptr<Connector>;

class connector_pointer :public std::shared_ptr<Connector> {
public:
	//让默认 connector 指针默认初始化自动 new ，而不是创建空指针。 
	connector_pointer() :std::shared_ptr<Connector>(new Connector) {};
	connector_pointer(Connector* p) :std::shared_ptr<Connector>(p) {};
};

using cptr = connector_pointer;

//typedef std::vector<cptr> connector_list;

class connector_list :public std::deque<cptr> {
public:
	connector_list() = default;

	// 此处不能使用 deque(int n) 构造函数，因为 share_ptr 默认生成空指针（不分配空间）
	connector_list(int n) {
		for (int i = 0; i < n; i++) {
			cptr tmp(new Connector());
			push_back(tmp);
		}
	}

	// 继承 deque 的大括号初始化法
	connector_list(std::initializer_list<cptr> li) :std::deque<cptr>(li) {}

	void show_value() {
		for (auto i : *this) {
			std::cout<<i->get_val()<<" ";
		}
	}

	void rshow_value() {
		for (auto i = rbegin(); i != rend(); i++) {
			std::cout << (*i)->get_val() << " ";
		}
	}

	void set_value(int n) {
		for (auto i : *this) {
			i->set_val(n & 1);
			n >>= 1;
		}
	}
};

class And_Gate :public Element {
	cptr i1;
	cptr i2;
	cptr out;
	virtual void work() {
		if (!out) {
			std::cerr<<"And_Gate: output dosen't exist!";//error
		} else if (i1&&i2) {
			out->set_val(i1->get_val() & i2->get_val());
		}
	}
public:
	And_Gate() :i1(nullptr), i2(nullptr), out(nullptr) {
		delay = 3.0;
	}
	And_Gate(cptr a, cptr b, cptr s) :i1(a), i2(b), out(s) {
		delay = 3.0;
		a->add_to(this);
		b->add_to(this);
		work();
	}

	//use this function only when it haven't started
	void set_pin(cptr a, cptr b, cptr c) {
		i1 = a;
		i2 = b;
		out = c;
		a->add_to(this);
		b->add_to(this);
		work();

	}
};

/* multiple input and gate */

class mAnd_Gate :public Element {
	connector_list input;
	cptr out;

	virtual void work() {
		if (!out) {
			std::cerr << "mAnd_Gate: output doesn't exist!";//error
		}
		else if (input.empty()) {
			std::cerr << "mAnd_Gate: empty input!";//error
		}
		else {
			bool res = true;
			for (auto i : input) {
				res &= (i->get_val());
			}
			out->set_val(res);
		}
	}
public:
	mAnd_Gate() :out(nullptr) {
		delay = 3.0;
	}
	mAnd_Gate(connector_list* in, cptr s) :input(*in), out(s) {
		delay = 3.0;
		for (auto i : input) {
			i->add_to(this);
		}
		work();
	}

	void set_pin(connector_list* in, cptr s) {
		input = *in;
		out = s;
		for (auto i : input) {
			i->add_to(this);
		}
		work();
	}
};

class Or_Gate :public Element {
	cptr i1;
	cptr i2;
	cptr out;

	virtual void work() {
		if (!out) {
			std::cerr << "Or_Gate: output doesn't exist!";//error
		} else if (i1&&i2) {
			out->set_val(i1->get_val() | i2->get_val());
		}
	}
public:
	Or_Gate() :i1(nullptr), i2(nullptr), out(nullptr) {
		delay = 5.0;
	}
	Or_Gate(cptr a, cptr b, cptr s) :i1(a), i2(b), out(s) {
		delay = 5.0;
		a->add_to(this);
		b->add_to(this);
		work();
	}

	void set_pin(cptr a, cptr b, cptr s) {
		i1 = a;
		i2 = b;
		out = s;
		a->add_to(this);
		b->add_to(this);
		work();
	}
};

/* multiple input or gate */

class mOr_Gate :public Element {
	connector_list input;
	cptr out;
	virtual void work() {
		if (!out) {
			std::cerr << "mOr_Gate: output doesn't exist!";//error
		}
		else if (input.empty()) {
			std::cerr << "mOr_Gate: empty input!";//error
		}
		else {
			bool res = false;
			for (auto i : input) {
				res |= (i->get_val());
			}
			out->set_val(res);
		}
	}
public:
	mOr_Gate() :out(nullptr) {
		delay = 5.0;
	}
	mOr_Gate(connector_list* in, cptr s) :input(*in), out(s) {
		delay = 5.0;
		for (auto i : input) {
			i->add_to(this);
		}
		work();
	}

	void set_pin(connector_list* in, cptr s) {
		input = *in;
		out = s;
		for (auto i : input) {
			i->add_to(this);
		}
		work();
	}
};

class Not_Gate : public Element {
	cptr in;
	cptr out;
	
	virtual void work() {
		if (!out) {
			std::cerr << "Not_Gate: output doesn't exist!";//error
		}
		else if (in) {
			out->set_val(!(in->get_val()));
		}
	}
public:
	Not_Gate() :in(nullptr), out(nullptr) {
		delay = 2.0;
	}
	Not_Gate(cptr i, cptr o):in(i),out(o) {
		delay = 2.0;
		in->add_to(this);
		work();
	}

	void set_pin(cptr i, cptr o) {
		in = i;
		out = o;
		in->add_to(this);
		work();
	}
};

/*

Abandonned code: real system always use and gate, or gate and not gate to implement xor gate.

class Xor_Gate :public Element {
	cptr i1;
	cptr i2;
	cptr out;

	virtual void work() {
		if (!out) {
			std::cerr << "Xor_Gate: output doesn't exist!";//error
		}
		else if (i1&&i2) {
			out->set_val(i1->get_val() ^ i2->get_val());
		}
	}
public:
	Xor_Gate(cptr a, cptr b, cptr s) :i1(a), i2(b), out(s) {
		delay = 7.0;
		a->add_to(this);
		b->add_to(this);
		work();
	}
};
*/

class Input :public Element {
	cptr in;
public:
	Input(cptr i) :in(i) {};
	void set_input(bool val) { in->set_val(val); }
};

class Output :public Element {
	cptr out;
public:
	Output(cptr o) :out(o) { out->add_to(this); }
	virtual void work() {}
};

class Probe :public Element {
	cptr target;
	std::string name;
public:
	Probe(std::string n, cptr tar) :name(n), target(tar) { 
		tar->add_to(this);
		work();
	}
	virtual void work();
};

class Query_List {
	typedef std::pair<double, std::deque<Element*>> segment;
	std::deque<segment> query_list;
	double current_time;
	void push_query(Element* ele, double time) {
		if (query_list.empty()) {
			std::deque<Element*> first_deq;
			first_deq.push_back(ele);
			query_list.push_back({ time,first_deq });
		} else if (time < query_list.front().first) {
			std::deque<Element*> first_deq;
			first_deq.push_back(ele);
			query_list.push_front({ time,first_deq });
		} else if (time > query_list.back().first) {
			std::deque<Element*> last_deq;
			last_deq.push_back(ele);
			query_list.push_back({ time,last_deq });
		} else {
			int size = query_list.size();
			auto list_iter = query_list.begin();
			while (list_iter != query_list.end()) {
				if (time == list_iter->first) {
					if(std::find(list_iter->second.begin(),list_iter->second.end(), ele)==list_iter->second.end())
						list_iter->second.push_back(ele);
					break;
				} else if (time > list_iter->first&&time < (list_iter+1)->first) {
					std::deque<Element*> new_deq;
					new_deq.push_back(ele);
					query_list.insert((list_iter+1), { time,new_deq });
					break;
				} else {
					++list_iter;
				}
			}
		}
	}
public:
	Query_List() :query_list(), current_time(0.0) {};
	void add_to_query_list(Element* ele, double delay) {
		push_query(ele, delay + current_time);
	}
	void run() {
		while (!query_list.empty()) {
			auto& tmp = query_list.front().second; //注意这里一定要用引用，否则在 work 过程中新添加进 query_list 的内容并不会改变 tmp 的值。
			current_time = query_list.front().first;
			while (!tmp.empty()) {
				tmp.front()->work();
				tmp.pop_front();
			}
			query_list.pop_front();
		}
		std::cout << "Current time = " << current_time << "\n";
	}
	void run(double t) {
		assert(t >= 0);
		double end_time = current_time + t;
		while (!query_list.empty()&&current_time<=end_time) {
			auto& tmp = query_list.front().second; //注意这里一定要用引用，否则在 work 过程中新添加进 query_list 的内容并不会改变 tmp 的值。
			current_time = query_list.front().first;
			while (!tmp.empty()) {
				tmp.front()->work();
				tmp.pop_front();
			}
			query_list.pop_front();
		}
		if (current_time < end_time) {
			current_time = end_time;
		}
		std::cout << "Current time = " << current_time << "\n";
	}
	double get_current_time() { return current_time; }
}Main_Query_List;

void call(std::vector<Element*>  ele) {
	for (auto i : ele) {
		Main_Query_List.add_to_query_list(i, i->get_delay());
	}
}

void Probe::work() {
	std::cout << name << ": value = " << target->get_val() << ", time = " << Main_Query_List.get_current_time() << "\n";
}