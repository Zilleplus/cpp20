#include<iostream>
#include<coroutine>
#include<exception> // for terminate()
#include <cassert>

template<typename CoroType>
struct CoroPromise {
	auto get_return_object() { // create the coroutine handle
		std::cout << "- promise: get_return_object \n";
		return std::coroutine_handle<CoroPromise<CoroType>>::from_promise(*this);
	}

	auto initial_suspend() { // start immediately?
		std::cout << "- promise: initial_suspend \n";
		return std::suspend_always{};
	}

	auto final_suspend() noexcept { // cleanup
		std::cout << "- promise: final_suspend\n";
		return std::suspend_always();
	}

	void unhandled_exception() {
		std::terminate();
	}

	void return_void() {
		std::cout << "promise return void\n";
	}
};

class CoroTask {
public:
	using promise_type = CoroPromise<CoroTask>;

private:
	std::coroutine_handle<promise_type> coroHdl;

public:
	CoroTask(auto h) : coroHdl(h) {
		std::cout << "- Corotask: construct\n";
	}

	~CoroTask() {
		std::cout << "- CoroTask: destruct\n";
		if (coroHdl) {
			coroHdl.destroy();
		}
	}

	CoroTask(const CoroTask&) = delete;
	CoroTask& operator=(const CoroTask&) = delete;

	// Api to resume the coroutine
	bool resume() const{
		std::cout << "- CoroTask: resume \n";
		if(!coroHdl)
		{
			return false; // nothing more to proces
		}

		coroHdl.resume();
		return !coroHdl.done();
	}

	bool done() const {
		return coroHdl.done();
	}
};

CoroTask sayHello()
{
	std::cout << "hello" << "\n";
	co_await std::suspend_always();
	std::cout << "world" << "\n";
	co_await std::suspend_always();
	std::cout << "!" << "\n";
}

int main() {
	CoroTask sayHelloTask = sayHello();
	
	sayHelloTask.resume();
	sayHelloTask.resume();

	return 0;
}