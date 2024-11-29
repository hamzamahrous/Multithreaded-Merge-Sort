#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

void merge(vector<int> &list, int left, int mid, int right) {
	int i, j, k, n1 = mid - left + 1, n2 = right - mid;
	vector<int> leftHalf(n1), rightHalf(n2);

	for (int i = 0; i < n1; i++) leftHalf[i] = list[left + i];
	for (int i = 0; i < n2; i++) rightHalf[i] = list[mid + 1 + i];

	i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (leftHalf[i] <= rightHalf[j]) {
			list[k] = leftHalf[i];
			i++;
		}
		else {
			list[k] = rightHalf[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		list[k] = leftHalf[i];
		i++, k++;
	}

	while (j < n2) {
		list[k] = rightHalf[j];
		j++, k++;
	}
}

void mergeSort(vector<int> &list, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		mergeSort(list, left, mid);
		mergeSort(list, mid + 1, right);

		merge(list, left, mid, right);
	}
}

void mergeSortThreaded(vector<int> &list, int left, int right, int numThreads, int depth = 0) {
	int maxDepth = log2(numThreads);

	if (right - left + 1 <= 5000 || depth >= maxDepth){
		mergeSort(list, left, right);
		return;
	}

	int mid = left + (right - left) / 2;

	thread t1(mergeSortThreaded, ref(list), left, mid, numThreads, depth + 1);
	thread t2(mergeSortThreaded, ref(list), mid + 1, right, numThreads, depth + 1);

	t1.join();
	t2.join();

	merge(list, left, mid, right);
}

int main() {
	unsigned int numThreads, maxThreads = thread::hardware_concurrency();

	cout << "The system supports " << maxThreads << " concurrent threads.\n";
	char choice;
	cout << "Do you want to enter the number of threads manually? (y/n): ";
	cin >> choice;

	if (choice == 'Y' || choice == 'y') {
		cout << "Enter the number of threads: "; cin >> numThreads;
		if (numThreads > maxThreads) {
			cout << "The number of threads cannot exceed " << maxThreads << ". Using the maximum available threads.\n";
			numThreads = maxThreads;
		}
	}
	else {
		numThreads = maxThreads;
	}

	cout << "Number of threads to use: " << numThreads << '\n';

	size_t n = 1000000;
	cout << "\nGenerating a random list with " << n << " elements for testing...\n";
	vector<int> data(n);
	srand(time(0));

	for (int i = 0; i < n; i++) {
		data[i] = rand();
	}

	// Measure time for sequential & multithreading merge sort
	vector<int> dataSequential = data;
	auto start = steady_clock::now();
	cout << "\nSorting the list using sequential merge sort...\n";
	mergeSort(dataSequential, 0, n - 1); // Sequential Sorting
	auto stop = steady_clock::now();
	auto durationSequential = duration_cast<milliseconds>(stop - start);
	cout << "Time taken for sequential merge sort: " << durationSequential.count() << " ms\n";

	vector<int> dataThreaded = data;
	start = steady_clock::now();
	cout << "\nSorting the list using multithreaded merge sort...\n";
	mergeSortThreaded(dataThreaded, 0, n - 1, numThreads); // Threaded Sorting
	stop = steady_clock::now();
	auto durationThreaded = duration_cast<milliseconds>(stop - start);
	cout << "Time taken for multithreaded merge sort using " << numThreads << " threads: " << durationThreaded.count() << " ms\n";

	return 0;
}

