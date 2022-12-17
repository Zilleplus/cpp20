#include<catch2/catch_test_macros.hpp>
#include<task.h>


static bool compute_1_completed = false;
static bool compute_1_prepare = false;
static bool compute_2_completed = false;
static bool compute_3_completed = false;
static bool compute_4_completed = false;

void reset_computes()
{
	compute_1_completed = false;
	compute_1_prepare = false;
	compute_2_completed = false;
	compute_3_completed = false;
	compute_4_completed = false;
}

tasklib::Task compute_1() {
	compute_1_prepare = true;
	auto a = co_await tasklib::Task::run([]() {return 1; });
	auto b = co_await tasklib::Task::run([]() {return 2; });
	compute_1_completed = true;

	co_return a + b;
}

int compute_2() {
	compute_2_completed = true;
	return 2;
}

void first_compute()
{
	compute_1();
	compute_2();
}

int compute_3() {
	compute_3_completed = true;
	return 1;
}

int compute_4() {
	compute_4_completed = true;
	return 2;
}

void second_computer()
{
	compute_3();
	compute_4();
}

void total_compute()
{
	first_compute();
	second_computer();
}

TEST_CASE("Given_Large_Computer_Interleave")
{
	// The idea is to interleave the comupute
	// so we call total_computer()
	// but we will execute the subtasks in the order:
	// compute_1()
	// compute_3()
	// compute_2()
	// compute_4()
	reset_computes();

	auto c = compute_1();
	REQUIRE(!compute_1_completed);
	REQUIRE(compute_1_prepare);
	c.resume(); // calc a
	c.resume(); // calc b
	REQUIRE(compute_1_completed);
}