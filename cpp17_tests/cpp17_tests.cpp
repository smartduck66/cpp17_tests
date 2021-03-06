// cpp17_tests.cpp : Tests de fonctionnalités C++17 effectués en partant du fichier cpp17detail.pdf
// Dernier build : 24 mars 2018
// La fonction Lambda RunAndMeasure mesure le temps d'exécution
// Les fonctions de // sont encore expérimentales sous VS15.6.4 - NE PAS OUBLIER DE MODIFIER LES PROPRIETES DE C++/language dans les propriétés du Projet
// A noter que : - le mode release est beaucoup plus rapide que le mode debug ; par contre, peu de gain en x64 versus x32
//				 - le parallélisme apporte un gain de 400% versus le mode séquentiel classique en tri, 500% en count, bien moins en copie (rien ou +/- 20% seulement) !!!
//               - le test sur les algorithmes accumulate, minmax_element, search et generate ne passent pas (pas encore implémentés ?)


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


inline void gen_sort(const int sort_type)
{
	// Génération du vecteur-test 
	vector<unsigned> v(25000000);
	random_device rd;
	mt19937 gen(rd());
	//poisson_distribution<> d(100000);
	uniform_int_distribution<> d(100000);	// Génération plus rapide que Poisson... et peut-être plus hétérogène (CVU mars 2018 - page 21) ?
	generate(begin(v), end(v), [&]() { return d(gen); });
	
	// Vecteur de destination pour la copie
	vector<unsigned> to_v(v.size());
	
	//for (int i = 0; i < 1000; ++i)
	//	cout << "i:" << i << " " << v[i]<<endl;
	
	// Tri effectué en fonction du type demandé
	switch (sort_type)
	{
	case 1:
		RunAndMeasure([&]() {
			int64_t n = count(v.begin(), v.end(), 46987220);
			cout << "46987220 found ? " << n << " fois        : ";
		});
		cout << "standard sequential sort       : ";
		RunAndMeasure([&]() {
			sort(v.begin(), v.end());
		});
		cout << "standard sequential copy       : ";
		RunAndMeasure([&]() {
			copy(v.begin(), v.end(), to_v.begin());
		});
				
		break;
	
	case 2:
		RunAndMeasure([&]() {
			int64_t n = count(std::execution::seq, v.begin(), v.end(), 46987220);
			cout << "46987220 found ? " << n << " fois        : ";
		});
		cout << "explicit sequential sort (seq) : ";
		RunAndMeasure([&]() {
			sort(std::execution::seq, v.begin(), v.end());
		});
		cout << "explicit sequential copy (seq) : ";
		RunAndMeasure([&]() {
			copy(std::execution::seq, v.begin(),v.end(), to_v.begin());
		});
		
		break;
	
	case 3:
		RunAndMeasure([&]() {
			int64_t n = count(std::execution::par, v.begin(), v.end(), 46987220);
			cout << "46987220 found ? " << n << " fois        : ";
		});
		cout << "parallel execution sort (par)  : ";
		RunAndMeasure([&]() {
			sort(std::execution::par, v.begin(), v.end());
		});
		cout << "parallel execution copy (par)  : ";
		RunAndMeasure([&]() {
			copy(std::execution::par, v.begin(), v.end(), to_v.begin());
		});
		break;

	case 4:
		RunAndMeasure([&]() {
			int64_t n = count(std::execution::par_unseq, v.begin(), v.end(), 46987220);
			cout << "46987220 found ? " << n << " fois        : ";
		});
		cout << "vectorization sort (par_unseq) :  ";
		RunAndMeasure([&]() {
			sort(std::execution::par_unseq, v.begin(), v.end());
		});
		cout << "vectorization copy (par_unseq) :  ";
		RunAndMeasure([&]() {
			copy(std::execution::par_unseq, v.begin(), v.end(), to_v.begin());
		});
		
		break;
		
	default:
		break;
	}

	cout << endl;
}

int main()
{
	cout << "Generation de vecteurs de 25 millions de valeurs : please wait..." << endl;

	// standard sequential 
	gen_sort(1);
	
	// explicitly sequential 
	gen_sort(2);

	// permitting parallel execution 
	gen_sort(3);
	
	// permitting vectorization as well 
	gen_sort(4);
				

	cout << endl;
	keep_window_open();
			
	return  0;
}