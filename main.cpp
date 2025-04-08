#include<iostream>
#include<iomanip>
#include<chrono>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;
using namespace chrono;

const int SIZE = 20;

// Fibonacci functions
unsigned long long int Fib(int n) {
	if (n == 0) {
		return 0;
	}
	else if (n == 1) {
		return 1;
	}
	else if (n >= 2) {
		return (Fib(n - 1) + Fib(n - 2));
	}
	else {
		return -1;
	}
}
unsigned long long int FibM(int n, unsigned long long int r[]) {
	if (r[n] != -1) {
		return r[n];
	}
	else {
		r[n] = (FibM(n - 1, r) + FibM(n - 2, r));
		return r[n];
	}
}

unsigned long long int FibMemoized(int n) {
	if (n == 0) {
		return 0;
	}
	else if (n == 1) {
		return 1;
	}
	else if (n >= 2) {
		unsigned long long int* r = new unsigned long long int[n + 1];

		for (int i = 0; i <= n; i++) {
			r[i] = -1;
		}

		r[0] = 0;
		r[1] = 1;

		unsigned long long int res = FibM(n, r);
		delete[] r;
		return res;
	}
	else {
		return -1;
	}
}

unsigned long long int FibBottomUp(int n) {
	if (n == 0) {
		return 0;
	}
	else if (n == 1) {
		return 1;
	}
	else if (n >= 2) {
		unsigned long long int* r = new unsigned long long int[n + 1];

		r[0] = 0;
		r[1] = 1;

		for (int i = 2; i <= n; i++) {
			r[i] = r[i - 1] + r[i - 2];
		}

		unsigned long long int res = r[n];
		delete[] r;
		return res;
	}
	else {
		return -1;
	}
}

void Parenthesization(int s[SIZE][SIZE], int i, int j) {
	if (i == j) {
		cout << "A" << i;
	}
	else {
		cout << "(";
		Parenthesization(s, i, s[i][j]);
		Parenthesization(s, s[i][j] + 1, j);
		cout << ")";
	}
}

// Matrix chain multiplication functions
int MatrixRecursive(int s [SIZE][SIZE], int m[SIZE][SIZE], int p[], int i, int j) {
	if (i == j) {
		return 0;
	}

	m[i][j] = 999999999;

	for (int k = i; k <= j - 1; k++) {
		int q = MatrixRecursive(s, m, p, i, k) + MatrixRecursive(s, m, p, k + 1, j) + (p[i - 1] * p[k] * p[j]);
	
		if (q < m[i][j]) {
			m[i][j] = q;
			s[i][j] = k;
		}
	}

	return m[i][j];
}

int Matrix(int n, int p[]) {
	int m[SIZE][SIZE];
	int s[SIZE][SIZE];
	int res = MatrixRecursive(s, m, p, 1, n);

	cout << "Optimal Parenthesization: ";
	Parenthesization(s, 1, n);
	cout << endl;

	return res;
}

int MatrixLookup(int s[SIZE][SIZE], int m[SIZE][SIZE], int p[], int i, int j) {
	if (m[i][j] < 999999999) {
		return m[i][j];
	}

	if (i == j) {
		m[i][j] = 0;
	}
	else {
		for (int k = i; k <= j - 1; k++) {
			int q = MatrixLookup(s, m, p, i, k) + MatrixLookup(s, m, p, k + 1, j) + (p[i - 1] * p[k] * p[j]);
		
			if (q < m[i][j]) {
				m[i][j] = q;
				s[i][j] = k;
			}
		}
	}

	return m[i][j];
}

int MatrixMemoized(int n, int p[]) {
	int m[SIZE][SIZE];
	int s[SIZE][SIZE];
	for (int i = 1; i <= n; i++) {
		for (int j = i; j <= n; j++) {
			m[i][j] = 999999999;
		}
	}

	int res = MatrixLookup(s, m, p, 1, n);

	cout << "Optimal Parenthesization: ";
	Parenthesization(s, 1, n);
	cout << endl;

	return res;
}

int MatrixBottomUp(int n, int p[]) {
	int m[SIZE][SIZE];
	int s[SIZE][SIZE];

	for (int i = 1; i <= n; i++) {
		m[i][i] = 0;
	}

	for (int l = 2; l <= n; l++) {
		for (int i = 1; i <= n - l + 1; i++) {
			int j = i + l - 1;
			m[i][j] = 999999999;

			for (int k = i; k <= j - 1; k++) {
				int q = m[i][k] + m[k + 1][j] + (p[i - 1] * p[k] * p[j]);

				if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}

	cout << "Optimal Parenthesization: ";
	Parenthesization(s, 1, n);
	cout << endl;

	return m[1][n];
}

// Main for calling each function
int main() {
	unsigned long long int res;
	int fibNums[SIZE] = {};
	int i = 0;

	for (int i = 0; i < SIZE; i++) {
		fibNums[i] = -1;
	}

	ifstream inFile;
	inFile.open("fibInput.txt");
	while (!inFile.eof()) {
		inFile >> fibNums[i];
		i++;
	}
	inFile.close();

	// Recursive fibonacci calls
	cout << "Recursive Fibonacci:" << endl;
	cout << setw(60) << setfill('-') << "" << endl;
	for (int i = 0; i < SIZE; i++) {
		if (fibNums[i] != -1) {
			const auto t1 = high_resolution_clock::now();
			res = Fib(fibNums[i]);
			const auto t2 = high_resolution_clock::now();
			const auto d = duration_cast<duration<double>> (t2 - t1).count();

			cout << "Time taken for fibonacci computation when n = " << fibNums[i] << ": " << d * 1000000 << " microseconds" << endl;
			cout << "Result: " << res << endl;
		}
	}
	cout << endl;

	// Memoized fibonacci calls
	cout << "Memoized Fibonacci:" << endl;
	cout << setw(60) << setfill('-') << "" << endl;
	for (int i = 0; i < SIZE; i++) {
		if (fibNums[i] != -1) {
			const auto t1 = high_resolution_clock::now();
			res = FibMemoized(fibNums[i]);
			const auto t2 = high_resolution_clock::now();
			const auto d = duration_cast<duration<double>> (t2 - t1).count();

			cout << "Time taken for fibonacci computation when n = " << fibNums[i] << ": " << d * 1000000 << " microseconds" << endl;
			cout << "Result: " << res << endl;
		}
	}
	cout << endl;

	// Bottom-up fibonacci calls
	cout << "Bottom-Up Fibonacci:" << endl;
	cout << setw(60) << setfill('-') << "" << endl;
	for (int i = 0; i < SIZE; i++) {
		if (fibNums[i] != -1) {
			const auto t1 = high_resolution_clock::now();
			res = FibBottomUp(fibNums[i]);
			const auto t2 = high_resolution_clock::now();
			const auto d = duration_cast<duration<double>> (t2 - t1).count();

			cout << "Time taken for fibonacci computation when n = " << fibNums[i] << ": " << d * 1000000 << " microseconds" << endl;
			cout << "Result: " << res << endl;
		}
	}
	cout << endl;

	string line;
	int p[SIZE] = {};
	int num;
	int j = 0;
	int n = 0;

	i = 0;
	inFile.open("matrixInput.txt");
	while (!inFile.eof()) {

		getline(inFile, line);
		stringstream ss(line);
		
		for (int i = 0; i < SIZE; i++) {
			p[i] = -1;
		}

		while (1) {
			ss >> num;
			if (!ss) {
				break;
			}

			if (j == 0) {
				n = num;
			}
			else {
				p[j - 1] = num;
			}

			j++;
		}
		j = 0;

		// Recursive matrix call
		cout << "Recursive Matrix:" << endl;
		cout << setw(60) << setfill('-') << "" << endl;

		auto t3 = high_resolution_clock::now();
		res = Matrix(n, p);
		auto t4 = high_resolution_clock::now();
		auto d1 = duration_cast<duration<double>> (t4 - t3).count();
		
		cout << "Minimum multiplications: " << res << endl;
		cout << "Time taken for matrix computation: " << d1 * 1000000 << " microseconds" << endl;
		cout << endl;

		// Memoized matrix calls
		cout << "Memoized Matrix:" << endl;
		cout << setw(60) << setfill('-') << "" << endl;

		t3 = high_resolution_clock::now();
		res = MatrixMemoized(n, p);
		t4 = high_resolution_clock::now();
		d1 = duration_cast<duration<double>> (t4 - t3).count();
		
		cout << "Minimum multiplications: " << res << endl;
		cout << "Time taken for matrix computation: " << d1 * 1000000 << " microseconds" << endl;
		cout << endl;

		// Bottom-up matrix calls
		cout << "Bottom-Up Matrix:" << endl;
		cout << setw(60) << setfill('-') << "" << endl;

		t3 = high_resolution_clock::now();
		res = MatrixBottomUp(n, p);
		t4 = high_resolution_clock::now();
		d1 = duration_cast<duration<double>> (t4 - t3).count();
		
		cout << "Minimum multiplications: " << res << endl;
		cout << "Time taken for matrix computation: " << d1 * 1000000 << " microseconds" << endl;
		cout << endl;
	}
	inFile.close();

	return 0;
}