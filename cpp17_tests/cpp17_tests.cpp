// cpp17_tests.cpp : Tests de fonctionnalités C++17 effectués en partant du fichier cpp17detail.pdf
// Dernier build : 24 mars 2018
// La fonction Lambda RunAndMeasure mesure le temps d'exécution
// Les fonctions de // sont encore expérimentales sous VS15.6.4 - NE PAS OUBLIER DE MODIFIER LES PROPRIETES DE C++/language dans les propriétés du Projet
// A noter que le mode release est beaucoup plus rapide que le mode debug ; par contre, peu de gain en x64 versus x32


// Librairie indispensable sous Windows et VC++
#include "stdafx.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>
#include <vector>
#include <execution>		// Code expérimental en VS15.6.4


using namespace std;

template <typename TFunc> void RunAndMeasure(TFunc func)
{
	const auto start = chrono::steady_clock::now();
	func();
	const auto end = chrono::steady_clock::now();
	cout << chrono::duration <double, milli>(end - start).count() << " ms\n";
}

inline void keep_window_open()				// Tirée de la bibliothèque de BS (on évite la "pollution" avec le header std_lib_facilities au cas où)
{
	cin.clear();
	cout << "Please enter a character to exit\n";
	char ch;
	cin >> ch;
	return;
}

int main()
{
	
	// Génération du vecteur-test
	vector<unsigned> v(10000000);
	random_device rd;
	mt19937 gen(rd());
	poisson_distribution<> d(100000);

	generate(begin(v), end(v), [&]() { return d(gen); });

	
	cout << "Tests de tri sur un vecteur de 10 millions d'occurences en utilisant les nouvelles politiques d'execution // de CPP17"<<endl<<endl;

	// standard sequential sort
	cout << "standard sequential sort :             ";
	RunAndMeasure([&]() {
		sort(v.begin(), v.end());
	});

	// explicitly sequential sort
	cout << "explicitly sequential sort (seq) :     ";
	RunAndMeasure([&]() {
		sort(std::execution::seq, v.begin(), v.end());
	});

	// permitting parallel execution
	cout << "permitting parallel execution (par) :  ";
	RunAndMeasure([&]() {
		sort(std::execution::par, v.begin(), v.end());
	});

	// permitting vectorization as well
	cout << "permitting vectorization (par_unseq) : ";
	RunAndMeasure([&]() {
		sort(std::execution::par_unseq, v.begin(), v.end());
	});

	cout << endl;
	keep_window_open();
	return  0;
}