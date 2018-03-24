// cpp17_tests.cpp : Tests C++17 effectués en partant du fichier cpp17detail.pdf
// 22 mars 2018
// La fonction Lambda RunAndMeasure mesure le temps d'éxécution
// Problèmes de compilation : 
// - std::seq, par et par_unseq seraient disponibles dans VC++ 15.7 (fait partie de algorithm/execution
// - sur coliru (GCC), pas de compilation non plus
//

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>
#include <vector>
#include <execution>

// #include <execution> : Visual Studio 15.6.4 = 24 mars 2018
// Erreur	C4996	'_Experimental_parallel_algorithms': warning STL4019 : Parallel algorithms support is experimental in this release of Visual C++.
// Object files and static libraries that include <execution> may need to be rebuilt to link with a future update of Visual C++, and programs using <execution> may not run on Windows XP.
// This warning will be removed when parallel algorithms support is completed in a future update.
// You can define _SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING to acknowledge that you have received this warning.cpp17_tests	


using namespace std;

template <typename TFunc> void RunAndMeasure(TFunc func)
{
	const auto start = std::chrono::steady_clock::now();
	func();
	const auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration <double, std::milli>(end - start).count() << " ms\n";
}

int main()
{
	
	
	vector<unsigned> v(5000000);
	random_device rd;
	mt19937 gen(rd());
	poisson_distribution<> d(10000);

	generate(begin(v), end(v), [&]() { return d(gen); });

	
	// standard sequential sort
	RunAndMeasure([&]() {
		sort(v.begin(), v.end());
	});

	// explicitly sequential sort
	RunAndMeasure([&]() {
		sort(std::execution::seq, v.begin(), v.end());
	});

	// permitting parallel execution
	RunAndMeasure([&]() {
		sort(std::execution::par, v.begin(), v.end());
	});

	// permitting vectorization as well
	RunAndMeasure([&]() {
		sort(std::execution::par_unseq, v.begin(), v.end());
	});

	
}