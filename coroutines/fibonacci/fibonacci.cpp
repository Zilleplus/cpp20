#include <coroutine>
#include <format>
#include <iostream>
#include <cassert>
class ReturnType {
public:
	struct promise_type {
		std::coroutine_handle<promise_type>  get_return_object() { return std::coroutine_handle<promise_type>::from_promise(*this); }

		void return_void() {}
		void unhandled_exception() {}

		std::suspend_always initial_suspend() {
			// Don't return the first fib number when called upon, just go to sleep.
			std::cout << "Initial suspend.\n";
			return {};
		};
		std::suspend_always final_suspend() noexcept {
			std::cout << "Final suspend.\n";
			return {};
		}
	};
private:
	std::coroutine_handle<promise_type> hdl;
	

public:
	bool resume() {
		if (!hdl.done())
		{
			hdl.resume();
		}

		return hdl.done();
	}

	ReturnType(auto h): hdl(h){}

	ReturnType(const ReturnType& other) = delete;
	ReturnType operator=(const ReturnType& other) = delete;

	ReturnType(ReturnType&& other) noexcept {
		hdl = other.hdl;
		other.hdl = nullptr;
	}
};

ReturnType Hello()
{
	std::cout << "hello world" << std::endl;
	co_await std::suspend_always(); // does not work either
}

int main() {
	auto series = Hello();
	series.resume();

	return 0;
}