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

		prime_status.reserve(n >> 1);
		for (auto i = 0; i < n; i += 2)
		{
			prime_status.push_back(true);
		}

		const auto max_factor = static_cast<int>(ceil(sqrt(n)));

		primes->push_back(2);

		for (auto i = 3; i < n; i += 2)
		{
			if (prime_status[i >> 1])
			{
				primes->push_back(i);

				if (i < max_factor)
				{
					for (auto j = i + i + i; j < n; j += i + i)
					{
						prime_status[j >> 1] = false;
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
		prime_status.reserve(n);
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
		std::vector<int> remaining_numbers;

		auto next_potential_prime = sqrt_primes->back() + 1;

		remaining_numbers.reserve((n - next_potential_prime) >> 1);

		if (next_potential_prime >> 1 << 1 == next_potential_prime)
		{
			next_potential_prime++;
		}

		//Get all odd numbers between next potential prime and n
		for (auto i = next_potential_prime; i < n; i += 2)
		{
			remaining_numbers.push_back(i);
		}

		//All numbers between max sqrt prime and n - All composites between max sqrt prime and n = All primes between max sqrt prime and n
		//TODO Check if iterator is faster than foreach loop
		for (auto prime : *sqrt_primes)
		{
			if (prime == 2)
			{
				continue;
			}

			auto i = prime + prime + prime;
			auto j = remaining_numbers.begin();
			while (i < n)
			{
				if (*j >= i)
				{
					if (*j == i)
					{
						j = remaining_numbers.erase(j);
					}

					i += prime + prime;
				}
				else
				{
					j += 1;
				}
			}
		}

		sqrt_primes->insert(sqrt_primes->end(), remaining_numbers.begin(), remaining_numbers.end());
		return sqrt_primes;
	}

	int* generate_primes(int n, int* length)
	{
		const auto count = n >> 1;
		auto* prime_status = static_cast<bool*>(malloc(count));

		for (auto i = 1; i < count; i++)
		{
			prime_status[i] = true;
		}

		const auto max_factor = static_cast<int>(ceil(sqrt(n)));
		auto prime_count = 1;

		for (auto i = 3; i < n; i += 2)
		{
			if (prime_status[i >> 1])
			{
				prime_count++;

				if (i < max_factor)
				{
					for (auto j = i + i + i; j < n; j += i + i)
					{
						prime_status[j >> 1] = false;
					}
				}
			}
		}

		*length = prime_count;

		int* primes = static_cast<int*>(malloc(prime_count * sizeof * primes));
		primes[0] = 2;

		for (auto i = 3, j = 1; i < n; i += 2)
		{
			if (prime_status[i >> 1])
			{
				primes[j++] = i;
			}
		}

		free(prime_status);

		return primes;
	}

	int* generate_primes_recursive(int n, int* length)
	{
		if (n <= 6)
		{
			int* primes;
			if (n >= 4)
			{
				primes = static_cast<int*>(malloc(2 * sizeof * primes));
				primes[0] = 2;
				primes[1] = 3;
				*length = 2;
			}
			else
			{
				primes = static_cast<int*>(malloc(sizeof * primes));
				primes[0] = 2;
				*length = 1;
			}

			return primes;
		}

		int sqrt_prime_count;
		auto* sqrt_primes = generate_primes_recursive(static_cast<int>(ceil(sqrt(n))), &sqrt_prime_count);

		auto next_potential_prime = sqrt_primes[sqrt_prime_count - 1] + 1;
		if (next_potential_prime >> 1 << 1 == next_potential_prime)
		{
			next_potential_prime++;
		}

		auto possible_prime_count = (n - next_potential_prime + 1) >> 1;

		int* remaining_numbers = static_cast<int*>(malloc(possible_prime_count * sizeof *remaining_numbers));

		//Get all odd numbers between next potential prime and n
		for (auto i = next_potential_prime, j = 0; i < n; i += 2, j++)
		{
			remaining_numbers[j] = i;
		}

		for (auto i = 1; i < sqrt_prime_count; i++)
		{
			const auto prime = sqrt_primes[i];
			auto factor = prime + prime + prime;

			auto position = 0;
			while (factor < n)
			{
				if (remaining_numbers[position] >= factor)
				{
					if (remaining_numbers[position] == factor)
					{
						remaining_numbers[position] = 0;
						position++;
						possible_prime_count--;
					}

					factor += prime + prime;
				}
				else
				{
					position++;
				}
			}
		}


		auto* const new_primes = static_cast<int*>(realloc(sqrt_primes, (sqrt_prime_count + possible_prime_count) * 4));
		if (new_primes == nullptr)
		{
			*length = sqrt_prime_count;
			return sqrt_primes;
		}

		*length = sqrt_prime_count + possible_prime_count;

		for (auto i = 0, j = sqrt_prime_count; j < sqrt_prime_count + possible_prime_count; i++)
		{
			if (remaining_numbers[i] != 0)
			{
				new_primes[j] = remaining_numbers[i];
				j++;
			}
		}

		return new_primes;
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