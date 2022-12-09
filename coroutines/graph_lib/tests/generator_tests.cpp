#include<catch2/catch_test_macros.hpp>
#include<generator.h>
#include<ranges>

glib::generator<int> numbers() {
	co_yield 1;
	co_yield 2;
}

TEST_CASE("Given_Fixed_Number_Generate")
{
	auto res = numbers();
	REQUIRE(!res.isDone());
	res.resume();
	REQUIRE(res.value() == 1);

	res.resume();
	REQUIRE(res.value() == 2);
	REQUIRE(!res.isDone());

	res.resume();
	REQUIRE(res.isDone());
}

TEST_CASE("Foreach_A_Number_Generator")
{
	int i = 1;
	// No enumerate in the range library yet.
	for (auto res : numbers())
	{
		REQUIRE(i == res);
		i = i + 1;
	}
}