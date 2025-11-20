#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// inputData: считывание данных
void inputData(float *p, int n);

// inputDataFromStream: считывание данных из указанного потокаф
void inputDataFromStream(std::istream &is, float *p, int n);

// average: подсчет среднего значения в массиве p
float average(const float *p, int n);

// minValue: нахождение минимального значения в массиве p
float minValue(const float *p, int n);

// maxValue: нахождение максимального значения в массиве p
float maxValue(const float *p, int n);

// printStat: вывод в поток базовых характеристик для массива
void printStat(std::ostream &os, float *p, int n);

// filterAboveAverage: возвращает указатель на динамический массив с элементами, большими среднего значения
float *filterAboveAverage(float *p, int n, int &newCount);

int main(int argc, char *argv[])
{
    int n;
    float *p;
    if (argc <= 1)
    {
        printf("=== Анализ сенсора ===\n");
        printf("Введите количество измерений: ");
        if (!(std::cin >> n) || n < 1)
        {
            std::cerr << "Некорректный ввод\n";
            return 1;
        }
        p = new float[n];

        // inputData(p, n);
        inputDataFromStream(std::cin, p, n);
    }
    else
    {
        std::ifstream file(*(argv + 1));
        if (!file.is_open())
        {
            std::cerr << "Не удалось открыть файл" << std::endl;
            return 1;
        }
        if (!(file >> n) || n < 1)
        {
            std::cerr << "Некорректный ввод\n";
            return 1;
        }
        p = new float[n];
        inputDataFromStream(file, p, n);
    }

    std::cout << std::endl;
    printStat(std::cout, p, n);
    std::cout << std::endl;

    int newCount;
    float *fp = filterAboveAverage(p, n, newCount);

    std::cout << "Значения выше среднего:\n";
    for (int i = 0; i < newCount; ++i)
    {
        std::cout << *(fp + i) << " ";
    }
    std::cout << std::endl;

    delete[] p;
    delete[] fp;

    return 0;
}

void inputData(float *p, int n)
{
    int i = 0;
    float tmp;

    while (i < n)
    {
        std::string input;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string str;
        while ((iss >> str) && (i < n))
        {
            std::stringstream substr(str);
            float num;
            if (substr >> num)
            {
                if (num <= 50 && num >= -50)
                {
                    *(p + i) = num;
                    ++i;
                }
                else
                {
                    printf("Введеное число вне диапазона [ -50 ; 50 ]\n");
                }
            }
            else
            {
                printf("Введена последовательность, которая не может быть конвертирована в число\n");
            }
        }
        if (n - i > 0)
        {
            printf("Осталось ввести %d значений\n", n - i);
        }
    }
}

void inputDataFromStream(std::istream &is, float *p, int n)
{
    int i = 0;
    float tmp;

    while (i < n)
    {
        std::string input;
        if (!std::getline(is, input))
        {
            std::cerr << "Удалось получить менее n значений\n";
            exit(1);
        }

        std::istringstream iss(input);
        std::string str;
        while ((iss >> str) && (i < n))
        {
            std::stringstream substr(str);
            float num;
            if (substr >> num)
            {
                if (num <= 50 && num >= -50)
                {
                    *(p + i) = num;
                    ++i;
                }
                else
                {
                    std::cerr << "Введеное число вне диапазона [ -50 ; 50 ]\n";
                }
            }
            else
            {
                std::cerr << "Введена последовательность, которая не может быть конвертирована в число\n";
            }
        }
        if (n - i > 0 && &is == &std::cin)
        {
            printf("Осталось ввести %d значений\n", n - i);
        }
    }
}

float average(const float *p, int n)
{
    float ans = 0;
    for (int i = 0; i < n; i++)
        ans += *(p + i);
    return ans / n;
}

float minValue(const float *p, int n)
{
    float minV = *p;
    for (int i = 0; i < n; i++)
        if (*(p + i) < minV)
            minV = *(p + i);
    return minV;
}

float maxValue(const float *p, int n)
{
    float maxV = *p;
    for (int i = 0; i < n; i++)
        if (*(p + i) > maxV)
            maxV = *(p + i);
    return maxV;
}

void printStat(std::ostream &os, float *p, int n)
{
    os << "Среднее значение: " << average(p, n) << std::endl;
    os << "Минимум: " << minValue(p, n) << std::endl;
    os << "Максимум: " << maxValue(p, n) << std::endl;
}

float *filterAboveAverage(float *p, int n, int &newCount)
{
    newCount = 0;
    float av = average(p, n);
    for (int i = 0; i < n; ++i)
        if (*(p + i) > av)
            ++newCount;
    if (newCount == 0)
    {
        std::cerr << "Все значения равны" << std::endl;
        return nullptr;
    }
    float *fp = new float[newCount];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (*(p + i) > av)
            *(fp + j++) = *(p + i);
    return fp;
}
