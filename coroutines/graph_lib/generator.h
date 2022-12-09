#include<coroutine>

namespace glib
{
	template<typename T>
	class generator {
	public:
		struct promise_type {
			T value;

			auto yield_value(T v) { // co_yield
				value = v;
				return std::suspend_always{};
			}

			auto  get_return_object() {
				return std::coroutine_handle<promise_type> ::from_promise(*this);
			}

			auto initial_suspend() { return std::suspend_always{}; };
			auto final_suspend() noexcept { return std::suspend_always{}; }
			void unhandled_exception() { }
			void return_void() {} // co_return
		};

	private:
		std::coroutine_handle<promise_type> hdl;

	public:
		generator(auto h) : hdl(h) {}
		generator() {
			if (hdl)
			{
				hdl.destroy();
			}
		}

		bool resume() {
			if (!hdl) { return false; }
			hdl.resume();

			return true; // we were able to continue
		}

		int value() const {
			return hdl.promise().value;
		}

		generator(const generator<T>& other) = delete;
		generator<T>& operator= (const generator<T>& other) = delete;

		generator(generator<T>&& other) noexcept : hdl(other.hdl) {
			other.hdl = nullptr;
		}

		bool isDone() const {
			if (!hdl) { return true; } // If no handle, then we are already done.
			return hdl.done();
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
				// Coroutine interface was potentially moved.
				// Or the coroutine is already done.
				assert(hdl != nullptr);
				return hdl.promise().value;
			}

			iterator operator++() {
				getNext();
				return *this;
			}

			// Used to compare to find out if we are the end.
			// Hdl will be null if the coroutine ended.
			bool operator == (const iterator& i) const = default;
		};

		iterator begin() const {
			if (!hdl || hdl.done())
			{
				return iterator{ nullptr };
			}

			iterator itor{ hdl };
			itor.getNext(); // resume for first value (or we could change the init suspend...)
			return itor;
		}

		iterator end() const {
			return iterator{ nullptr };
		}
	};
}