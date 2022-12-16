#include <coroutine>
#include <memory>

namespace rgen {
	class generator {
	public:
		class promise_type {
		private:
			int value_;
			std::unique_ptr<generator> inner_generator_ = nullptr;
		public:
			bool has_inner_generator() const { return inner_generator_ != nullptr; }

			int value() const {
				return has_inner_generator() ? inner_generator_->value() : value_;
			}

			auto yield_value(int v) // co_yield
			{
				value_ = v;
				return std::suspend_always{};
			}

			auto yield_value(generator&& v) // co_yield
			{
				inner_generator_ = std::make_unique<generator>(std::move(v));
				inner_generator_->resume(); // get a value ready in the inner generator;
				return std::suspend_always{};
			}

			bool resume_inner_generator()
			{
				return inner_generator_->resume();
			}

			auto get_return_object()
			{
				return std::coroutine_handle<promise_type>::from_promise(*this);
			}

			auto initial_suspend() { return std::suspend_always{}; }
			auto final_suspend() noexcept { return std::suspend_always{}; }

			void unhandled_exception() {}
			void return_void() {}
		};

	private:
		std::coroutine_handle<promise_type> hdl;

	public:
		generator(generator&& other) noexcept : hdl(other.hdl)
		{
			other.hdl == nullptr; // otherwise we would destroy coro ...
		}

		generator(std::coroutine_handle<promise_type> h) : hdl(h) {}

		generator() {
			if (hdl)
			{
				hdl.destroy();
			}
		}

		bool resume() {
			// check if inner generator still have values...
			if (hdl &&
				hdl.promise().has_inner_generator()
				&& hdl.promise().resume_inner_generator())
			{
				return true;
			}

			if (!hdl || hdl.done()) { return false; }
			hdl.resume();

			return true; // we were able to continue
		}

		bool isDone() const {
			if (!hdl) { return true; } // If no handle, then we are already done.
			return hdl.done();
		}

		int value() const {
			return hdl.promise().value();
		}
	};
}