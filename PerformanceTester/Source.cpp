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

void main()
{
	using std::filesystem::path;
	using std::chrono::high_resolution_clock;
	using std::chrono::duration;
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
	
	auto t1 = high_resolution_clock::now();
	auto cpp_primes = primes_cpp::generate_primes(max_value);
	auto t2 = high_resolution_clock::now();

	const std::ofstream out_cpp("results\\primescpp.txt");
	auto* std_backup = std::cout.rdbuf(out_cpp.rdbuf());

	std::cout << '2';
	
	for (auto i = 1; i < cpp_primes->size(); i++)
	{
		std::cout << std::endl << (*cpp_primes)[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << "cpp vector time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;

	t1 = high_resolution_clock::now();
	auto cpp_mp_primes = primes_cpp::generate_primes_multi_threaded(max_value);
	t2 = high_resolution_clock::now();

	const std::ofstream out_cpp_mp("results\\primescppmp.txt");
	std_backup = std::cout.rdbuf(out_cpp_mp.rdbuf());

	std::cout << '2';

	for (auto i = 1; i < cpp_mp_primes->size(); i++)
	{
		std::cout << std::endl << (*cpp_mp_primes)[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << "cpp vector multi threaded time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;

	t1 = high_resolution_clock::now();
	auto cpp_recursive_primes = primes_cpp::generate_primes_recursive(max_value);
	t2 = high_resolution_clock::now();

	const std::ofstream out_cpp_recursive("results\\primescprecursive.txt");
	std_backup = std::cout.rdbuf(out_cpp_recursive.rdbuf());

	std::cout << '2';

	for (auto i = 1; i < cpp_recursive_primes->size(); i++)
	{
		std::cout << std::endl << (*cpp_recursive_primes)[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << "cpp vector recursive time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;
	
	auto c_count = 0;
	t1 = high_resolution_clock::now();
	auto* c_primes = primes_cpp::generate_primes(max_value, &c_count);
	t2 = high_resolution_clock::now();

	const std::ofstream c_out("results\\primesc.txt");
	std_backup = std::cout.rdbuf(c_out.rdbuf());

	std::cout << '2';

	for (auto i = 1; i < c_count; i++)
	{
		std::cout << std::endl << c_primes[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << "cpp pointer time: " << duration<double, std::milli>(t2 - t1).count() << "ms" << std::endl;
	
	auto asm_count = 0;
	t1 = high_resolution_clock::now();
	auto* asm_primes = primes_asm64::generate_primes(max_value, &asm_count);
	t2 = high_resolution_clock::now();

	const std::ofstream asm_out("results\\primesasm.txt");
	std_backup = std::cout.rdbuf(asm_out.rdbuf());

	std::cout << '2';

	for (auto i = 1; i < asm_count; i++)
	{
		std::cout << std::endl << asm_primes[i];
	}

	std::cout.rdbuf(std_backup);
	std::cout << "x86_64 asm time: " << duration<double, std::milli>(t2 - t1).count() << "ms";
}
