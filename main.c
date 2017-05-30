#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "string.h"
#include "time.h"
#define simb ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
#define N 100
/* ключ комлексный */
/* сортировка подсчетом */
typedef struct _comp
{
    int a;
    int b;
} comp;

typedef struct _temp
{
    comp _key;
    char *_data;
} temp;
int size_arr(char c[])
{
    int i = 0;
    while (c[i] != '\0')
        i++;
    return i;
}
int toInt(char c[], int *index)
{
    int a = 0;
    int i = *index;
    int p = 1;
    while (c[i] != '+' && c[i] != '-')
    {
        if (c[i] >= '0' && c[i] <= '9')
        {
            a += p * (c[i] - 48);
            p *= 10;
        }
        i--;
        if (i < 0)
            break;
    }
    if (i >= 0)
    {
        if (c[i] == '-')
            a = -a;
    }
    *index = i;
    return a;
}
comp ReadKey(char c[])
{
    comp t;
    t.a = 0;
    t.b = 0;
    /* Читаем первую цифру, добавляем в мнимую часть */
    int length = size_arr(c);
    for (int i = length - 1; i >= 0; i--)
    {
        if (c[i] == 'i')
            t.b = toInt(c, &i);
        else
            t.a = toInt(c, &i);
    }
    return t;
}
char *ReadData(FILE *stream)
{
    char *str = NULL;
    char ch = '\0';
    int i = 0;
    int y = 0;
    while (!simb)
    {
        ch = fgetc(stream);
    }
    do
    {
        str = (char *)realloc(str, sizeof(char) * (i + 1));
        str[i] = ch;
        ch = fgetc(stream);
        i++;
    } while (simb || ch == ' ');
    str = (char *)realloc(str, sizeof(char) * (i + 1));
    str[i] = '\0';
    return str;
}
int modComp(comp x)
{
    return x.a * x.a + x.b * x.b;
}
int isSorted(temp a[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        if (modComp(a[i]._key) > modComp(a[i + 1]._key))
            return 0;
    }
    return 1;
}
int binSearch(temp a[], int n, temp x)
{
    int lhs = 0, rhs = n - 1, mid;

    if (rhs <= 0)
        return -1;
    int xMod = modComp(x._key);
    while (lhs < rhs)
    {
        mid = (lhs + rhs) / 2;
        int midMod = modComp(a[mid]._key);
        if (xMod == midMod)
            return mid;
        else if (xMod > midMod)
            lhs = mid + 1;
        else
            rhs = mid;
    }

    if (modComp(a[rhs]._key) == xMod)
        return rhs;
    return -1;
}
void swapTemp(temp *a, temp *b)
{
    temp t = *a;
    *a = *b;
    *b = t;
    return;
}
void scramble(temp *a, const int size)
{
    int i, j, k;
    srand((unsigned int)time(0));
    for (k = 0; k < size; k++)
    {
        i = rand() % size;
        j = rand() % size;
        swapTemp(&a[i], &a[j]);
    }
}
void CompToStr(comp x, char ch[])
{
    int n = 0;
    if (x.a != 0)
        sprintf(ch, "%d", x.a);
    if (x.b != 0)
    {
        if (x.a != 0)
            n = size_arr(ch);
        if (x.b > 0)
            sprintf(ch + n, "+%di", x.b);
        else
            sprintf(ch + n, "%di", x.b);
    }
    if (x.a == x.b && x.a == 0)
        sprintf(ch, "%d", 0);
    return;
}
void printTable(temp a[], const int size)
{
    printf("+---------+------------------------------------------------+\n");
    printf("|   Ключ  |                    Значение                    |\n");
    printf("+---------+------------------------------------------------+\n");

    for (int i = 0; i < size; i++)
    {
        char c[10];
        CompToStr(a[i]._key, c);
        printf("|%4s     |%48s|\n", c, a[i]._data);
    }

    printf("+---------+------------------------------------------------+\n");
}
void reverse(temp a[], int size)
{
    int i, j;
    for (int i = 0; i < size / 2; i++)
        swapTemp(&a[i], &a[size - i - 1]);
    return;
}
void sort(temp a[], const int size)
{
    int count[size];
    temp b[size];
    for (int i = 0; i < size; i++)
        count[i] = 0;

    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
            if (modComp(a[i]._key) > modComp(a[j]._key))
                count[i]++;
            else
                count[j]++;
    for (int i = 0; i < size; i++)
        b[count[i]] = a[i];

    for (int i = 0; i < size; i++)
        a[i] = b[i];
    return;
}
int main()
{
    FILE *in = fopen("test.txt", "r");
    if (in == NULL)
    {
        printf("Неудается открыть файл\n");
        return 1;
    }
    temp arr[N];
    temp tmp;
    int index = 0;
    char key[10];
    /* Enter DATA */
    while (fscanf(in, "%s", key) == 1)
    {
        comp C = ReadKey(key);
        char *data = ReadData(in);
        tmp._key = C;
        tmp._data = data;
        arr[index] = tmp;
        index++;
    }
    int SIZE = index;
    fclose(in);
    /* end enter data */
    int action;
    do
    {
        printf("Меню\n");
        printf("1) Печать\n");
        printf("2) Двоичный поиск\n");
        printf("3) Сортировка\n");
        printf("4) Перемешивание\n");
        printf("5) Реверс\n");
        printf("6) Выход\n");
        printf("Выберите действие\n");
        scanf("%d", &action);
        switch (action)
        {
        case 1:
        {
            printTable(arr, SIZE);
        }
        break;

        case 2:
        {
            if (!isSorted(arr, SIZE))
                printf("Ошибка. Таблица не отсортирована\n");
            else
            {
                printf("Введите ключ: ");
                scanf("%s", key);
                tmp._key = ReadKey(key);
                int i = binSearch(arr, SIZE, tmp);
                if (i > -1)
                    printf("Найдена строка: %s\n", arr[i]._data);
                else
                    printf("Строка с таким ключом не найдена\n");
            }
        }
        break;

        case 3:
        {
            sort(arr, SIZE);
        }
        break;

        case 4:
        {
            scramble(arr, SIZE);
        }
        break;

        case 5:
        {
            reverse(arr, SIZE);
        }
        break;

        case 6:
            break;

        default:
        {
            printf("Ошибка. Такого пункта меню не существует\n");
        }
        break;
        }
    } while (action != 6);
    return 0;
}