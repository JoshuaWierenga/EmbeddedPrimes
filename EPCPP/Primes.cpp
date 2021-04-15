#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace primes_cpp
{
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

				if (i < max_factor)
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

	void process_multiples(int n, std::vector<bool>& prime_status, int i_low, int i_high)
	{
		for (auto i = i_low; i < i_high; i++)
		{
			if (prime_status[i])
			{
				for (auto j = i + i; j < n; j += i)
				{
					prime_status[j] = false;
				}
			}
		}
	}

	//TODO Fix, currently this adds some non primes to the result
	std::unique_ptr<std::vector<int>> generate_primes_multi_threaded(const int n)
	{
		std::vector<bool> prime_status;
		std::unique_ptr<std::vector<int>> primes(new std::vector<int>);

		//TODO Use one loop if possible
		for (auto i = 0; i < n; i++)
		{
			prime_status.push_back(true);
		}

		const auto max_factor_part = static_cast<int>(ceil(sqrt(n))) >> 4;
		std::vector<std::thread> threads;

		threads.emplace_back(process_multiples, n, std::ref(prime_status), 2, max_factor_part);
		threads.emplace_back(process_multiples, n, std::ref(prime_status), max_factor_part, max_factor_part << 1);
		threads.emplace_back(process_multiples, n, std::ref(prime_status), max_factor_part << 1, max_factor_part << 2);
		threads.emplace_back(process_multiples, n, std::ref(prime_status), max_factor_part << 2, max_factor_part << 3);

		for (auto& th : threads)
		{
			th.join();
		}

		for (auto i = 2; i < n; i++)
		{
			if (prime_status[i])
			{
				primes->push_back(i);
			}
		}

		return primes;
	}

	std::unique_ptr<std::vector<int>> generate_primes_recursive(const int n)
	{
		if (n <= 6)
		{
			std::unique_ptr<std::vector<int>> primes(new std::vector<int>);
			if (n >= 3)
			{
				primes->push_back(2);
				if (n >= 4)
				{
					primes->push_back(3);
				}
			}

			return primes;
		}

		auto sqrt_primes = generate_primes_recursive(static_cast<int>(ceil(sqrt(n))));
		//TODO Set initial size to prevent potential reallocating
		std::vector<int> remaining_numbers;

		auto max_prime = 2;
		if (!sqrt_primes->empty())
		{
			max_prime = sqrt_primes->back() + 1;
		}

		//TODO Skip every even number
		//Get all numbers between max sqrt prime and n
		for (auto i = max_prime; i < n; i++)
		{
			remaining_numbers.push_back(i);
		}

		//All numbers between max sqrt prime and n - All composites between max sqrt prime and n = All primes between max sqrt prime and n
		for (auto prime : *sqrt_primes)
		{
			//TODO Figure out if the while loop needs a condition for j, works as is so only reason would be performance
			//TODO Replace j with iterator
			auto i = prime + prime, j = 0;
			while (i < n)
			{
				if (remaining_numbers[j] >= i)
				{
					if (remaining_numbers[j] == i)
					{
						remaining_numbers.erase(remaining_numbers.begin() + j);
						j++;
					}
					
					i += prime;
				}
				else
				{
					j++;
				}
			}
		}
		sqrt_primes->insert(sqrt_primes->end(), remaining_numbers.begin(), remaining_numbers.end());
		return sqrt_primes;
	}

	int* generate_primes(int n, int* length)
	{
		auto* prime_status = static_cast<bool*>(malloc(n));

		for (auto i = 0; i < n; i++)
		{
			prime_status[i] = true;
		}

		const auto max_factor = static_cast<int>(ceil(sqrt(n)));
		auto prime_count = 0;

		for (auto i = 2; i < n; i++)
		{
			if (prime_status[i])
			{
				prime_count++;

				if (i < max_factor)
				{
					for (auto j = i + i; j < n; j += i)
					{
						prime_status[j] = false;
					}
				}
			}
		}

		*length = prime_count;

		int* primes = static_cast<int*>(malloc(prime_count * sizeof * primes));

		for (auto i = 2, j = 0; i < n; i++)
		{
			if (prime_status[i])
			{
				primes[j++] = i;
			}
		}

		return primes;
	}

	/*int* generate_primes_recursive(int n, int* length)
	{

	}*/
}

/*void main()
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
}*/