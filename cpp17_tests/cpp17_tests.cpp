// cpp17_tests.cpp : Tests C++17 effectués en partant du fichier cpp17detail.pdf
// 22 mars 2018
// La fonction Lambda RunAndMeasure mesure le temps d'éxécution
// Problèmes de compilation : 
// - std::seq, par et par_unseq seraient disponibles dans VC++ 15.7 (fait partie de algorithm/execution
// - sur coliru (GCC), pas de compilation non plus
//

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>
#include <vector>
//#include "std_lib_facilities.h"



template <typename TFunc> void RunAndMeasure(TFunc func)
{
	const auto start = std::chrono::steady_clock::now();
	func();
	const auto end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration <double, std::milli>(end - start).count() << "
		ms\n";
}

int main()
{
	
	
	std::vector<unsigned> v(5000000);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::poisson_distribution<> d(10000);

	std::generate(begin(v), end(v), [&]() { return d(gen); });

	
	// standard sequential sort
	RunAndMeasure([&]() {
		std::sort(v.begin(), v.end());
	});

	// explicitly sequential sort
	RunAndMeasure([&]() {
		std::sort(std::seq, v.begin(), v.end());
	});

	// permitting parallel execution
	RunAndMeasure([&]() {
		std::sort(std::par, v.begin(), v.end());
	});

	// permitting vectorization as well
	RunAndMeasure([&]() {
		std::sort(std::par_unseq, v.begin(), v.end());
	});

	
}