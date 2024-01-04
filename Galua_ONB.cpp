#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>


std::vector<int> toPol(const std::string& a) {
    std::vector<int> c;
    for (char digit : a) {
        c.push_back(static_cast<int>(digit - '0'));
    }
    return c;
}

std::string toBin(const std::vector<int>& a) {
    std::string c;
    for (int digit : a) {
        c += std::to_string(digit);
    }
    return c;
}

std::vector<int> toFullLength(const std::vector<int>& a) {
    std::vector<int> a1(a);
    while (a1.size() < 293) {
        a1.insert(a1.begin(), 0);
    }
    return a1;
}


std::vector<int>* toOneLength(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> a1(a);
    std::vector<int> b1(b);
    std::vector<int>* result = new std::vector<int>[2];
    while (a1.size() > b1.size()) {
        a1.insert(a1.begin(), 0);
    }
    while (b1.size() > a1.size()) {
        b1.insert(b1.begin(), 0);
    }
    result[0] = a1;
    result[1] = b1;
    return result;
}

std::vector<int> leftShift(const std::vector<int>& a, int b) {
    std::vector<int> a1(a);
    for (int i = 0; i < b; ++i) {
        int temp = a1[0];
        a1.erase(a1.begin());
        a1.push_back(temp);
    }
    return a1;

}

std::vector<int> rightShift(const std::vector<int>& a, int b) {
    std::vector<int> a1(a);
    for (int i = 0; i < b; ++i) {
        int temp = a1.back();
        a1.pop_back();
        a1.insert(a1.begin(), temp);
    }
    return a1;
}

void delZeros(std::vector<int>& a) {
    while (a.size() > 1 && a[0] == 0) {
        a.erase(a.begin());
    }
}


std::vector<int> Add(const std::string& a, const std::string& b) {
    std::vector<int>* result = toOneLength(toPol(a), toPol(b));
    std::vector<int> a1 = result[0];
    std::vector<int> b1 = result[1];

    std::reverse(a1.begin(), a1.end());
    std::reverse(b1.begin(), b1.end());
    std::vector<int> c;
    for (size_t i = 0; i < a1.size(); ++i) {
        c.push_back(a1[i] ^ b1[i]);
    }
    std::reverse(c.begin(), c.end());
    delZeros(c);
    return c;
}

std::vector<std::vector<int>> MulMatrix(int m) {
    int* array = new int[m];
    int p = 2 * m + 1;
    array[0] = 1;

    for (int i = 1; i < m; ++i) {
        array[i] = (array[i - 1] * 2) % p;
    }

    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < m; ++i) {
        std::vector<int> temp;
        for (int j = 0; j < m; ++j) {
            if (((array[i]) + (array[j])) % p == 1 ||
                ((((array[i]) - (array[j])) % p) + p) % p == 1 ||
                (((-(array[i]) + (array[j])) % p) + p) % p == 1 ||
                (((-(array[i]) - (array[j])) % p) + p) % p == 1) {
                temp.push_back(1);
            }
            else {
                temp.push_back(0);
            }
        }
        matrix.push_back(temp);
    }
    return matrix;
}

std::vector<int> Mul(const std::vector<int>& a, const std::vector<int>& b, const std::vector<std::vector<int>>& matrix) {
    std::vector<int> a1 = toFullLength(a);
    std::vector<int> b1 = toFullLength(b);
    std::vector<int> c;
    for (int k = 0; k < 293; ++k) {
        std::vector<int> temp;
        for (int i = 0; i < 293; ++i) {
            int t = 0;
            for (int j = 0; j < 293; ++j) {
                t ^= (a1[j] & matrix[j][i]);
            }
            temp.push_back(t);
        }
        int sum = 0;
        for (int i = 0; i < 293; ++i) {
            sum ^= (temp[i] & b1[i]);
        }
        c.push_back(sum);
        a1 = leftShift(a1, 1);
        b1 = leftShift(b1, 1);
    }
    delZeros(c);
    return c;
}

std::vector<int> Square(const std::vector<int>& a) {
    std::vector<int> a1 = toFullLength(a);
    a1 = rightShift(a1, 1);
    delZeros(a1);
    return a1;
}

std::vector<int> Pow(const std::vector<int>& a, const std::string& b, const std::vector<std::vector<int>>& matrix) {
    std::vector<int> c(293, 1);
    for (int i = 0; i < 293; ++i) {
        if (b.at(i) == '1') {
            c = Mul(c, a, matrix);
        }
        if (i != 292) {
            c = Square(c);
        }
    }
    return c;
}

int Trace(const std::vector<int>& a) {
    int t = 0;
    for (int digit : a) {
        t ^= digit;
    }
    return t;
}


std::vector<int> Inverse(const std::vector<int>& a, const std::vector<std::vector<int>>& matrix) {
    std::string c(293 - 1, '1');
    c += '0';
    return Pow(a, c, matrix);
}


int main() {
    std::vector<std::vector<int>> matrix = MulMatrix(293);

    std::string p1_str, p2_str, n;
    std::cout << "Enter the first binary number: ";
    std::cin >> p1_str;
    std::vector<int> p1 = toPol(p1_str);

    std::cout << "Enter the second binary number: ";
    std::cin >> p2_str;
    std::vector<int> p2 = toPol(p2_str);

    std::cout << "Enter the exponent number: ";
    std::cin >> n;
    std::cout << std::endl;

   
    std::cout << "Sum: " << toBin(Add(p1_str, p2_str)) << std::endl;
    std::cout << std::endl;
    std::cout << "Mul: " << toBin(Mul(p1, p2, matrix)) << std::endl;
    std::cout << std::endl;
    std::cout << "Square: " << toBin(Square(p1)) << std::endl;
    std::cout << std::endl;
    std::cout << "Trace: " << Trace(p1) << std::endl;
    std::cout << std::endl;
    std::cout << "Pow (a^n): " << toBin(Pow(p1, n, matrix)) << std::endl;
    std::cout << std::endl;
    std::cout << "Inverse: " << toBin(Inverse(p1, matrix)) << std::endl;
    std::cout << std::endl;


    std::cout << "Measuring time for operations..." << std::endl;

    const int iter = 25;

    long long total_add_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string result_add = toBin(Add(p1_str, p2_str));
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_add_time += duration.count();
    }
    double average_add_time = static_cast<double>(total_add_time) / iter;
    std::cout << "Average time add: " << average_add_time << " nanoseconds\n";


    long long total_mul_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string result_mul = toBin(Mul(p1, p2, matrix));
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_mul_time += duration.count();
    }
    double average_mul_time = static_cast<double>(total_mul_time) / iter;
    std::cout << "Average time mul: " << average_mul_time << " nanoseconds\n";


    long long total_square_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string result_square = toBin(Square(p1));
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_square_time += duration.count();
    }
    double average_square_time = static_cast<double>(total_square_time) / iter;
    std::cout << "Average time square: " << average_square_time << " nanoseconds\n";


    long long total_pow_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string result_pow = toBin(Pow(p1, n, matrix));
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_pow_time += duration.count();
    }
    double average_pow_time = static_cast<double>(total_pow_time) / iter;
    std::cout << "Average time pow: " << average_pow_time << " nanoseconds\n";

    long long total_trace_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        int result_trace = Trace(p1);
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_trace_time += duration.count();
    }
    double average_trace_time = static_cast<double>(total_trace_time) / iter;
    std::cout << "Average time trace: " << average_trace_time << " nanoseconds\n";


    long long total_inverse_time = 0;
    for (int i = 0; i < iter; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string result_inverse = toBin(Inverse(p1, matrix));
        auto endTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        total_inverse_time += duration.count();
    }
    double average_inverse_time = static_cast<double>(total_inverse_time) / iter;
    std::cout << "Average time inverse: " << average_inverse_time << " nanoseconds\n";


    return 0;
}