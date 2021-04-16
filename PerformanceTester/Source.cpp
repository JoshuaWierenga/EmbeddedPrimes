#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>

//TODO There has to be a better way to access these functions but I have limited cpp knowledge
namespace primes_cpp
{
	extern std::unique_ptr<std::vector<int>> generate_primes(int n);
	std::unique_ptr<std::vector<int>> generate_primes_multi_threaded(int n);
	std::unique_ptr<std::vector<int>> generate_primes_recursive(int n);
	extern int* generate_primes(int n, int* length);
}

namespace primes_asm64
{
	extern "C" int* generate_primes(int n, int* length);
}

typedef std::unique_ptr<std::vector<int>> (*vector_function)(int n);
typedef int* (*pointer_function)(int n, int* length);

void evaluate_vector_algorithm(const int max_value, const vector_function algorithm, const std::string name, const std::string file_name)
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;

	const auto t1 = high_resolution_clock::now();
	const auto primes = algorithm(max_value);
	const auto t2 = high_resolution_clock::now();

	const std::ofstream out_cpp("results\\primes" + file_name + ".txt");
	auto* std_backup = std::cout.rdbuf(out_cpp.rdbuf());

	std::cout << '2';
	
	for (auto i = 1; i < primes->size(); i++)
	{
		std::cout << std::endl << (*primes)[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << name << " time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;
}

void evaluate_pointer_algorithm(const int max_value, const pointer_function algorithm, const std::string name, const std::string file_name)
{
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;

	int prime_count;
	const auto t1 = high_resolution_clock::now();
	auto* const primes = algorithm(max_value, &prime_count);
	const auto t2 = high_resolution_clock::now();

	const std::ofstream out_cpp("results\\primes" + file_name + ".txt");
	auto* std_backup = std::cout.rdbuf(out_cpp.rdbuf());

	std::cout << '2';

	for (auto i = 1; i < prime_count; i++)
	{
		std::cout << std::endl << primes[i];
	}

	free(primes);

	std::cout.rdbuf(std_backup);
	std::cout << name << " time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;
}

void main()
{
	using std::filesystem::path;
	using std::chrono::milliseconds;

	//104730 gives 10000 primes
	auto max_value = 104730;

	auto output = path(".") / "results";
	if (exists(output) && !is_directory(output))
	{
		std::cout << "Error: File could Results exists, this directly is needed to output results";
		return;
	}
	if (!exists(output) && !create_directory(output))
	{
		std::cout << "Error: Count not create Results folder";
		return;
	}

#ifdef _DEBUG
	std::cout << "Debug";
#else
	std::cout << "Release";
#endif 

	std::cout << " performance for generating all primes less than " << max_value << std::endl;
	
	evaluate_vector_algorithm(max_value, primes_cpp::generate_primes, "cpp vector", "cppvector");
	evaluate_vector_algorithm(max_value, primes_cpp::generate_primes_multi_threaded, "cpp vector multi threaded", "cppvectormp");
	evaluate_vector_algorithm(max_value, primes_cpp::generate_primes_recursive, "cpp vector recursive", "cppvectorrecursive");

	evaluate_pointer_algorithm(max_value, primes_cpp::generate_primes, "cpp pointer", "cpppointer");
	evaluate_pointer_algorithm(max_value, primes_asm64::generate_primes, "x86_64 asm", "asm");
}

