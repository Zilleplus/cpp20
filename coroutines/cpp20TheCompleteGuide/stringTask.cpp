#include<iostream>
#include<thread>
#include<chrono>
#include<coroutine>

class StringTask {
public:
	struct promise_type {
		std::string result;

		void return_value(const auto& value) { // reaction on coreturn
			result = value; // store value locally (could we move it instead???)
		}

		auto get_return_object() {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}

		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate_handler(); }
	};

private:
	std::coroutine_handle<promise_type> coroHdl;

public:
	StringTask(auto h) : coroHdl{h}{}
	~StringTask() {
		if (coroHdl)
		{
			coroHdl.destroy();
		}
	}

	StringTask(StringTask const&) = delete; // no copying
	StringTask& operator=(StringTask const&) = delete;

	bool resume() const {
		if (!coroHdl) {
			return false;
		}
		coroHdl.resume();
		return !coroHdl.done();
	}

	std::string getResult() const {
		return coroHdl.promise().result;
	}
};

StringTask computeInSteps()
{
	std::string ret;
	ret += "hello ";
	co_await std::suspend_always();
	ret += "world";
	co_await std::suspend_always();
	ret += "!";
	co_return ret;
}

int main() {
	using namespace std::literals;

	StringTask task = computeInSteps();

	while (task.resume())
	{
		std::this_thread::sleep_for(500ms);
	}

	std::cout << "result: " << task.getResult() << '\n';
}