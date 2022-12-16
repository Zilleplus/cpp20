#include<catch2/catch_test_macros.hpp>
#include<generator.h>

rgen::generator inner(int offset)
{
	for (int i = offset; i < 10; ++i)
	{
		co_yield i;
	}
}

rgen::generator outer()
{
	co_yield inner(0);
	co_yield inner(10);
}

TEST_CASE("Given_Inner_Loop")
{
	std::vector<int> res;

	auto coro = inner(0);

	while (coro.resume())
	{
		if (!coro.isDone())
		{
			auto val = coro.value();
			res.push_back(val);
		}
	}

	std::vector<int> expected;
	for (int i = 0; i < 10; ++i)
	{
		expected.push_back(i);
	}

	REQUIRE(res == expected);
}

TEST_CASE("Given_Outer_Loop")
{
	std::vector<int> res;

	auto coro = outer();

	while (coro.resume())
	{
		if (!coro.isDone())
		{
			auto val = coro.value();
			res.push_back(val);
		}
	}

	std::vector<int> expected;
	for (int i = 0; i < 10; ++i)
	{
		expected.push_back(i);
	}
	for (int i = 10; i < 20; ++i)
	{
		expected.push_back(i);
	}

	REQUIRE(res == expected);
}
