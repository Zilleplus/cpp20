#include<format>
#include<iostream>
#include<optional>
#include <coroutine>
#include <vector>

template<typename T>
struct Generator {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    T value_;
    std::exception_ptr exception_;

    Generator get_return_object() {
      return Generator(handle_type::from_promise(*this));
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { exception_ = std::current_exception(); }
    template<std::convertible_to<T> From> // C++20 concept
    std::suspend_always yield_value(From&& from) {
      value_ = std::forward<From>(from);
      return {};
    }
    void return_void() {}
  };

  handle_type h_;

  Generator(handle_type h) : h_(h) {}
  ~Generator() { h_.destroy(); }
  explicit operator bool() {
    fill();
    return !h_.done();
  }
  T operator()() {
    fill();
    full_ = false;
    return std::move(h_.promise().value_);
  }

private:
  bool full_ = false;

  void fill() {
    if (!full_) {
      h_();
      if (h_.promise().exception_)
        std::rethrow_exception(h_.promise().exception_);
      full_ = true;
    }
  }
};

struct Node {
  Node(int id, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
    : id(id), left(std::move(left)), right(std::move(right))
  {}

  int id;
  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  std::vector<Node*> Childs() {
    std::vector<Node*> out;

    if (left != nullptr)
    {
      out.push_back(left.get());
    }
    if (right != nullptr)
    {
      out.push_back(right.get());
    }

    return out;
  }
};

std::unique_ptr<Node> make_node(int id, std::unique_ptr<Node> left = nullptr, std::unique_ptr<Node> right = nullptr) {
  return std::make_unique<Node>(0, std::move(left), std::move(right));
}

Generator<std::vector<Node*>> vis(Node* n);

void breadth_first_search(const std::unique_ptr<Node>& root) {
  auto init = vis(root.get());
  std::vector<Generator<std::vector<Node*>>> childs = { init };
  std::vector<Generator<std::vector<Node*>>> next_childs;

  for (auto& c_gen : childs)
  {
    for (auto c : c_gen())
    {
      next_childs.push_back(vis(c));
    }
  }

  childs.clear();
  std::swap(next_childs, childs);
}

Generator<std::vector<Node*>> vis(Node* n) {
  if (n == nullptr)
  {
    // return;
  }
  std::cout << "visiting node id=" << n->id << std::endl;

  std::vector<Node*> out;
  if (n->left != nullptr) {
    out.push_back(n->left.get());
  }
  if (n->right != nullptr) {
    out.push_back(n->right.get());
  }

  co_yield std::move(out);
}

int main() {
  std::cout << "Breath first search" << std::endl;

  auto n2 = make_node(2, make_node(0), make_node(1));
  auto n5 = make_node(5, make_node(3), make_node(4));
  auto n6 = make_node(6, std::move(n2), std::move(n5));

  breadth_first_search(n6);
}