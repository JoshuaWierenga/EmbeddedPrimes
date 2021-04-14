#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

std::unique_ptr<std::vector<int>> generate_primes(const int n)
{
	std::vector<bool> prime_status;	
	std::unique_ptr<std::vector<int>> primes(new std::vector<int>);

	//TODO Use one loop if possible
	for (auto i = 0; i < n; i++)
	{
		prime_status.push_back(true);
	}

	const auto max_factor = static_cast<int>(ceil(sqrt(n)));

	for (auto i = 2; i < n; i++)
	{
		if (prime_status[i])
		{
			primes->push_back(i);
			
			if (i <= max_factor)
			{
				for (auto j = i + i; j < n; j += i)
				{
					prime_status[j] = false;
				}
			}
		}
	}
	
	return primes;
}

void main()
{
	//104730 gives 10000 primes
	const auto primes = generate_primes(104730);

	//std::ofstream out("primes.txt");
	//std::cout.rdbuf(out.rdbuf());
	
	//std::cout << "1, 2, 2";
	std::cout << '2';
	
	for (auto i = 1; i < primes->size(); i++)
	{
		//std::cout << std::endl << i + 1 << ", " << (*primes)[i] << ", " << (*primes)[i] - (*primes)[i - 1];
		//std::cout << std::endl << (*primes)[i];
		std::cout << ", " << (*primes)[i];
	}
}