#include<coroutine>
#include<iostream>
#include<vector>
#include<thread>
#include<cassert>

class IntGen;

class IntGen {
public:
	struct promise_type {
		int current_value; // last value from the co_yield

		auto yield_value(int value) {
			current_value = value; // store value locally
			return std::suspend_always{}; // suspend the coroutine
		}

		auto get_return_object() {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}

		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		void return_void() {}
	};

private:
	std::coroutine_handle<promise_type> coroHdl;
public:
	IntGen(auto h) : coroHdl{ h } {}

	~IntGen() {
		if (coroHdl) {
			coroHdl.destroy();
		}
	}

	IntGen(IntGen const&) = delete;
	IntGen(IntGen&& rhs) noexcept : coroHdl(rhs.coroHdl) {
		rhs.coroHdl = nullptr; // make sure we only have 1 handle
	}
	IntGen& operator=(IntGen const&) = delete; // no copying

	bool resume() const {
		assert(coroHdl != nullptr);// Coroutine interface was potentially moved.
		if (!coroHdl)
		{
			return false;
		}
		coroHdl.resume();
		return !coroHdl.done();
	}

	int getValue() const {
		return coroHdl.promise().current_value;
	}

	// stuff to make for(auto a : gen) work

	struct iterator {
		std::coroutine_handle<promise_type> hdl;
		iterator(auto p) : hdl{ p } { }

		void getNext() {
			if (hdl) {
				hdl.resume();
				if (hdl.done())
				{
					hdl = nullptr;
				}
			}
		}

		int operator*() const {
			assert(hdl != nullptr);// Coroutine interface was potentially moved.
			return hdl.promise().current_value;
		}

		iterator operator++() {
			getNext();
			return *this;
		}

		bool operator == (const iterator& i) const = default;
	};

	iterator begin() const {
		if (!coroHdl || coroHdl.done())
		{
			return iterator{ nullptr };
		}

		iterator itor{ coroHdl };
		itor.getNext(); // resume for first value (or we could change the init suspend...)
		return itor;
	}

	iterator end() const {
		return iterator{ nullptr };
	}
};

template<typename T>
IntGen loopOver(const T& coll)
{
	for (int elem : coll) {
		std::cout << "- yield " << elem << '\n';
		co_yield elem; // calls yield_value(elem) on promise
		std::cout << "- resume\n";
	}
}

int main() {
	using namespace std::literals;

	std::vector<int> coll{ 0,8,15, 33, 42,77 };
	IntGen gen = loopOver(coll);

	std::cout << "start while: \n";
	while (gen.resume()) {
		std::cout << "main(): value=" << gen.getValue() << '\n';
		std::this_thread::sleep_for(0.5s);
	}
	std::cout << "end while \n";

	IntGen genFor = loopOver(coll);
	std::cout << "start for: \n";
	for(auto val : genFor)
	{ 
		std::cout << "main(): value=" << gen.getValue() << '\n';
		std::this_thread::sleep_for(0.5s);
		
	}
	std::cout << "end for";

	return 0;
}