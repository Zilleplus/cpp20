#include <coroutine>
#include <format>
#include <iostream>
#include <cassert>
class ReturnType {
public:
	struct promise_type {
		std::coroutine_handle<promise_type>  get_return_object()
		{
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		int value;

		auto yield_value(auto v) { // co_yield
			value = v;
			return std::suspend_always{};
		}

		void return_void() {} // co_return

		void unhandled_exception() {}

		std::suspend_always initial_suspend() {
			return {};
		};
		std::suspend_always final_suspend() noexcept {
			return {};
		}
	};
private:
	std::coroutine_handle<promise_type> hdl;


public:
	bool resume() {
		if (!hdl)
		{
			return false;
		}
		hdl.resume();

		return true;
	}

	int value() const {
		return hdl.promise().value;
	}

	ReturnType(auto h) : hdl(h) {}

	ReturnType(const ReturnType& other) = delete;
	ReturnType operator=(const ReturnType& other) = delete;

	ReturnType(ReturnType&& other) noexcept {
		hdl = other.hdl;
		other.hdl = nullptr;
	}
};

ReturnType Hello(int n)
{
	if (n < 1)
	{
		co_return;
	}
	co_yield 1;

	if (n < 2)
	{
		co_return;
	}
	co_yield 1;

	int current = 1;
	int previous = 1;
	for (int i = 0; i < n - 2; ++i)
	{
		auto new_value = current + previous;
		co_yield new_value;
		previous = current;
		current = new_value;
	}
}

int main() {
	auto series = Hello(10);
	auto blah = series.resume();
	while (series.resume())
	{
		std::cout << series.value() << "\n";
	}

	return 0;
}