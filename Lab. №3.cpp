#include <iostream>
#include <cmath>
#include <windows.h>
#include <time.h>

#pragma warning(disable: 4996)

#define PI 3.1415926535
#define SKO 1
#define MX 0
#define EPS 0.00001

using namespace std;

double function(double x)
{
    double result = 1.0 / (SKO * powf(2.0 * PI, 0.5)) * expf(-1.0 * (powf(x - MX, 2.0) / (2.0 * powf(SKO, 2.0))));
    return result;
}

double integral(double x_start, double x_end)
{
    double x = x_start;
    double delta = 0.0;
    double s_current = 0.0;
    double s_previous = 0.0;
    int intervals = 1;
    int i = 0;
    do
    {
        s_previous = s_current;
        s_current = 0.0;
        intervals *= 2;
        delta = (x_end - x_start) / intervals;
        for (i = 1, x = x_start; i <= intervals; i++, x += delta)
        {
            s_current += function(x);
        }
        s_current *= delta;
    } while (fabs(s_previous - s_current) > EPS);
    return s_current;
}

class cube
{
private:
    int sides;
    double* probability, * ort;
public:
    cube()
    {
        sides = 6;
        probability = new double[sides];
        ort = new double[sides];
        double sum = 0;
        for (int i = 0; i < sides - 1; i++)
        {
            probability[i] = 1.0 / sides;
            sum += probability[i];
            ort[i] = sum;
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;
    }
    cube(int cube_sides)
    {
        if (cube_sides > 1)
        {
            sides = cube_sides;
        }
        else
        {
            sides = 6;
        }
        probability = new double[sides];
        ort = new double[sides];
        double sum = 0;
        for (int i = 0; i < sides - 1; i++)
        {
            probability[i] = 1.0 / sides;
            sum += probability[i];
            ort[i] = sum;
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;
    }
    cube(int cube_sides, int dominant_side, double probability_side)
    {
        double sum = 0;
        if (cube_sides > 1 && dominant_side > 0 && dominant_side <= cube_sides && probability_side > 0 && probability_side < 1)
        {
            sides = cube_sides;
            probability = new double[sides];
            ort = new double[sides];
            double p = (1 - probability_side) / (sides - 1);
            for (int i = 0; i < sides - 1; i++)
            {
                if ((i + 1) == dominant_side)
                {
                    probability[i] = probability_side;
                }
                else
                {
                    probability[i] = p;
                }
                sum += probability[i];
                ort[i] = sum;
            }
        }
        else
        {
            sides = 6;
            probability = new double[sides];
            ort = new double[sides];
            for (int i = 0; i < sides - 1; i++)
            {
                probability[i] = 1.0 / sides;
                sum += probability[i];
                ort[i] = sum;
            }
        }
        probability[sides - 1] = 1 - sum;
        ort[sides - 1] = 1;

    }
    cube(int cube_sides, double x_start, double x_end)
    {
        if (cube_sides > 1 && x_start < x_end)
        {
            sides = cube_sides;
            probability = new double[sides];
            ort = new double[sides];
            double sum = 0.0;
            double delta = (x_end - x_start) / sides;
            double x = x_start;
            double coefficient = 0.0;
            for (int i = 0; i < sides; i++, x += delta)
            {
                probability[i] = integral(x, x + delta);
                sum += probability[i];
            }
            coefficient = 1 / sum;
            sum = 0.0;
            for (int i = 0; i < sides - 1; i++)
            {
                probability[i] *= coefficient;
                sum += probability[i];
                ort[i] = sum;
            }
        }
        else
        {
            sides = 6;
            probability = new double[sides];
            ort = new double[sides];
            double sum = 0;
            for (int i = 0; i < sides - 1; i++)
            {
                probability[i] = 1.0 / sides;
                sum += probability[i];
                ort[i] = sum;
            }
            probability[sides - 1] = 1 - sum;
            ort[sides - 1] = 1;
        }
    }
    cube(cube& object)
    {
        sides = object.sides;
        probability = new double[sides];
        ort = new double[sides];
        for (int i = 0; i < sides; i++)
        {
            probability[i] = object.probability[i];
            ort[i] = object.ort[i];
        }
    }
    ~cube()
    {
        delete[]probability;
        delete[]ort;
    }
    int get_sides()
    {
        return sides;
    }
    double get_probability_side(int side_number)
    {
        if (side_number > 0 && side_number <= sides)
        {
            return (probability[side_number - 1]);
        }
        return 0;
    }
    void output()
    {
        cout << "Количество сторон: " << sides << endl;
        for (int i = 0; i < sides; i++)
        {
            cout << i + 1 << "     " << probability[i] << endl;
        }
    }
    int roll()
    {
        double x = (double)rand() / RAND_MAX;
        for (int i = 0; i < sides; i++)
        {
            if (x <= ort[i])
            {
                return (i + 1);
            }
        }
        return 1;
    }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(NULL));
    int i = 0;
    int N = 150;
    int number = 0;
    double* array = new double[N];
    double x_start = -3.0;
    double x_end = 3.0;
    double left_line = 0.0;
    cube a(20, x_start, x_end);
    a.output();
    double delta = (x_end - x_start) / a.get_sides();
    FILE* output = fopen("output.txt", "w");
    if (output == nullptr)
    {
        cout << "[Ошибка]: не удалось открыть файл для записи" << endl;
        return 1;
    }
    for (i = 0; i < N; i++)
    {
        number = a.roll();
        left_line = x_start + (number - 1) * delta;
        array[i] = delta * (double)rand() / (double)RAND_MAX + left_line;
        cout << array[i] << "  ";
        fprintf(output, "%f ", array[i]);
    }
    fclose(output);
    delete[]array;
    return 0;
}