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
		//TODO Hardcode a few extra cases, n = 3, 4?
		if (n == 2)
		{
			return std::make_unique<std::vector<int>>();
		}
		if (n <= 4)
		{
			std::unique_ptr<std::vector<int>> primes(new std::vector<int>);
			primes->push_back(2);
			primes->push_back(3);

			return primes;
		}

		auto sqrt_primes = generate_primes_recursive(static_cast<int>(ceil(sqrt(n))));
		std::vector<int> remaining_numbers;

		auto max_prime = 2;
		if (!sqrt_primes->empty())
		{
			max_prime = sqrt_primes->back() + 1;
		}

		//Get all numbers between max sqrt prime and n
		for (auto i = max_prime; i < n; i++)
		{
			remaining_numbers.push_back(i);
		}

		//All numbers between max sqrt prime and n - All composites between max sqrt prime and n = All primes between max sqrt prime and n
		for (auto prime : *sqrt_primes)
		{
			auto j = 0;
			//TODO Rewrite as a single while loop with [j] < i: j++; [j] == i: i+=prime, j++ and delete; [j] > i: i+=prime, j++
			//might also be able to use bigger jumps of size prime for j under specific conditions
			//TODO Start at first multiple of prime > max_prime, this should reduce time spent incrementing i back reasonable sizes each iteration
			for (auto i = prime + prime; i < n; i += prime)
			{
				for (; j < remaining_numbers.size(); j++)
				{
					if (remaining_numbers[j] > i)
					{
						break;
					}
					if (remaining_numbers[j] == i)
					{
						remaining_numbers.erase(remaining_numbers.begin() + j);
						break;
					}
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