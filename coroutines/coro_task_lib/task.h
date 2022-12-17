#include<coroutine>
#include<queue>
#include<iostream>

namespace tasklib {

	class Task {
	public:
		struct promise_type {
			int val;
			std::coroutine_handle<promise_type>  get_return_object() { return std::coroutine_handle<promise_type>::from_promise(*this); }

			void return_value(int v) { val = v; }
			void unhandled_exception() {}

			std::suspend_never initial_suspend() { return {}; };
			std::suspend_always final_suspend() noexcept { return {}; }

			bool can_resume() const {
				// idea: allocate something in heap and 
				// pass it through the handle from awaitable to promise.
				return true; // Need to be synced with awaitable somehow.
			}

			auto await_transform(auto&& awaitable) noexcept {
				// Looks like perfect forwarding, but this 
				// this wil always require a move.
				return std::move(awaitable);
			}
		};


		template<typename T>
		struct awaitable {
			T future;

			awaitable(T&& future) : future(std::forward<T>(future)) {}
			std::coroutine_handle<> hdl; // do not call destroy on this!

			bool await_ready() {
				// Called before suspending, if true, it will not suspend.
				return false;
			}

			bool is_ready() const {
				// Who will call this?
				return true;
			}

			void await_suspend(std::coroutine_handle<> h) { hdl = h; }
			auto await_resume() {
				// return the result!
				return 1;
			}
		};

		template<typename T>
		static awaitable<T> run(T&& lambda) { return awaitable<T>(std::forward<T>(lambda)); }

	private:
		std::coroutine_handle<promise_type> hdl;

	public:
		Task(std::coroutine_handle<promise_type> h) : hdl(h) {}

		Task(Task&& other) : hdl(other.hdl) {
			other.hdl = nullptr;
		}

		Task() {
			if (hdl)
			{
				hdl.destroy();
			}
		}

		bool resume() {
			if (!hdl || hdl.done()) { return false; }
			hdl.resume();

			return true; // we were able to continue
		}

		bool isDone() const {
			if (!hdl) { return true; } // If no handle, then we are already done.
			return hdl.done();
		}

		int value() const {
			return 0;
		}
	};

	// Context to execute the tasks in.
	// class Executor {
	// 	std::queue<Task> to_do;
	// public:
	// 	// execute one task, return true if able to execute task, return false if not more work left.
	// 	bool run_one_task();

	// 	bool has_more_work() const;
	// };

	// bool Executor::run_one_task()
	// {
	// 	return false;
	// }

	// bool Executor::has_more_work() const
	// {
	// 	return false;
	// }
}