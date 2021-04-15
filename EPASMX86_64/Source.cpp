#include <iostream>
#include <fstream>

namespace primes_asm64
{
	extern "C" int* generate_primes(int n, int* length);
}

/*void main()
{
	auto primes_count = 0;
	//104730 gives 10000 primes
	auto* const primes = primes_asm64::generate_primes(104730, &primes_count);

	//std::ofstream out("primes.txt");
	//std::cout.rdbuf(out.rdbuf());
	
	//std::cout << "1, 2, 2";
	std::cout << '2';
	
	for (auto i = 1; i < primes_count; i++)
	{
		//std::cout << std::endl << i + 1 << ", " << (*primes)[i] << ", " << (*primes)[i] - (*primes)[i - static_cast<long long>(1)];
		//std::cout << std::endl << primes[i];
		std::cout << ", " << primes[i];
	}
}*/