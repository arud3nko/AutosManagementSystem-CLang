#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define MAX_BUFFER_SIZE 128
#define SOURCE_FILE "/Users/egorrudenko/CLionProjects/Practice5/database.txt"
#define SOURCE_FILE_NAME "database.txt"
#define TEMP_FILE "/Users/egorrudenko/CLionProjects/Practice5/temp.txt"

#define INCORRECT_INPUT 0
#define CORRECT_INPUT_NUMS 2

#define MODEL_POSITIONS 7
#define NEW_MODEL_POSITIONS 5
#define CURRENT_YEAR 2022
#define TAX_LOW 7
#define TAX_MID 14
#define TAX_HIGH 30

#define SWITCH_FIRST 1
#define SWITCH_SECOND 2
#define SWITCH_THIRD 3

#define MENU_VIEW_ALL_ROWS 1
#define MENU_ADD_NEW_ROW 2
#define MENU_DELETE_ROW 3
#define MENU_SORT 4
#define MENU_SEARCH 5
#define MENU_EXIT 6

#define SEARCH_BY_NAME 1
#define SEARCH_BY_YEAR 2
#define SEARCH_BY_HP 3
#define SEARCH_BY_MILEAGE 4


#define SORT_BY_YEAR 1
#define SORT_BY_HP 2
#define SORT_BY_MILEAGE 3
#define SORT_BY_ENGINE_CAPACITY 4
#define SORT_BY_TAX 5

#define BACKSPACE_KEY 8
#define START_CHAR_RANGE 32
#define END_CHAR_RANGE 126

typedef struct
{
    char name[MAX_BUFFER_SIZE];
    int year;
    int engine_capacity;
    int horse_power;
    int mileage;
    int tax;
    int age;
} model;

void IncorrectNotificator()
{
    printf("---------------------------------------------------\n");
    printf("Введено некорректное значение! Попробуйте заново...\n");
    printf("---------------------------------------------------\n");
}

int ValidInput()
{
    int input = 0;
    char term = {0};

    if ((scanf("%d%c", &input, &term) != CORRECT_INPUT_NUMS) || (term != '\n'))
    {
        while((term = getchar()) != '\n' && (term != EOF));
        IncorrectNotificator();
        return INCORRECT_INPUT;
    }

    return input;
}

int CountNumberOfExistingRows()
{
    char tmp;
    int count = 1;
    FILE* source;

    source = fopen(SOURCE_FILE, "r");

    for (tmp = fgetc(source); tmp != EOF; tmp = fgetc(source))
    {
        if (tmp == '\n')
        {
        count += 1;
        }
    }
    fclose(source);

    return count;
}

void ReadExistingRows(model* autos)
{
    int i = 0;
    FILE* source;

    source = fopen(SOURCE_FILE, "r");

    while (fscanf(source, "%s %d %d %d %d %d %d",
                  autos[i].name,
                  &autos[i].year,
                  &autos[i].engine_capacity,
                  &autos[i].horse_power,
                  &autos[i].mileage,
                  &autos[i].tax,
                  &autos[i].age) == 7)
    {
        ++i;
    }

    fclose(source);
}

void PrintExistingRows(model* autos, int number_of_rows)
{
    printf("ID | Марка | Год выпуска | Объем дв. | Мощность | Пробег | Налог | Возраст\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < number_of_rows; ++i)
    {
        printf("[%d] %8s %8d      %5d      %5d      %5d     %5d    %5d\n",
               i+1,
               autos[i].name,
               autos[i].year,
               autos[i].engine_capacity,
               autos[i].horse_power,
               autos[i].mileage,
               autos[i].tax,
               autos[i].age);
    }
}

void PrintOneModel(model autos)
{
    printf("%8s %8d      %5d      %5d      %5d     %5d    %5d\n",
           autos.name,
           autos.year,
           autos.engine_capacity,
           autos.horse_power,
           autos.mileage,
           autos.tax,
           autos.age);
}


char* StringInput()
{
    char* userStr = (char*) malloc(sizeof(char));
    char curChar = 0;
    int curSize = 1;

    userStr[0] = '\0';

    while (curChar != '\n')
    {
        curChar = getchar();

        int deltaVal = 0;
        int lengthDif = 0;

        if (curChar == BACKSPACE_KEY)
        {
            deltaVal = -1;
            lengthDif = 1;
        }
        else
        {
            if (curChar >= START_CHAR_RANGE && curChar <= END_CHAR_RANGE)
            {
                deltaVal = 1;
                lengthDif = 2;
            }
            else
            {
                continue;
            }
        }

        int newSize = curSize + deltaVal;

        if (newSize == 0)
        {
            continue;
        }

        char* tmpStr = (char*) malloc(newSize * sizeof(char));

        if (tmpStr)
        {
            for (int i=0; i < newSize - lengthDif; ++i)
            {
                tmpStr[i] = userStr[i];
            }
            if (curChar != BACKSPACE_KEY)
            {
                tmpStr[newSize - 2] = curChar;
            }
            tmpStr[newSize - 1] = '\0';
            free(userStr);
            userStr = tmpStr;
            curSize = newSize;
        }
        else
        {
            printf("ERROR!");
            break;
        }
    }

    return userStr;
}

void AddNewModel()
{
    FILE* source;
    model new_auto;
    char* string_tmp = NULL;
    int int_tmp = 0;

    printf("Марка автомобиля: ");
    string_tmp = StringInput();
    strcpy(new_auto.name, string_tmp);

    printf("Год выпуска: ");
    new_auto.year = ValidInput();

    printf("Объем двигателя (л.): ");
    new_auto.engine_capacity = ValidInput();

    printf("Мощность двигателя (л. с.): ");
    new_auto.horse_power = ValidInput();

    printf("Пробег (км.): ");
    new_auto.mileage = ValidInput();

    if (new_auto.horse_power < 100)
    {
        new_auto.tax = TAX_LOW * new_auto.horse_power;
    }
    else if ((new_auto.horse_power >= 100) && (new_auto.horse_power <= 200))
    {
        new_auto.tax = TAX_MID * new_auto.horse_power;
    }
    else
    {
        new_auto.tax = TAX_HIGH * new_auto.horse_power;
    }

    new_auto.age = CURRENT_YEAR - new_auto.year;

    source = fopen(SOURCE_FILE, "a");

    fprintf(source, "\n%s %d %d %d %d %d %d",
            new_auto.name,
            new_auto.year,
            new_auto.engine_capacity,
            new_auto.horse_power,
            new_auto.mileage,
            new_auto.tax,
            new_auto.age);

    fclose(source);
}

void RemoveSpecificModel(int number_of_rows)
{
    FILE* source;
    FILE* temp;
    char curr;
    int del = 0;
    int line_number = 0;

    printf("Введите номер удаляемой позиции: ");

    while ((del = ValidInput()) > number_of_rows)
    {
        IncorrectNotificator();
        del = ValidInput();
    }

    source = fopen(SOURCE_FILE, "r");
    temp = fopen(TEMP_FILE, "w");

    curr = getc(source);

    if (curr != EOF)
    {
        line_number = 1;
    }

    while (1)
    {
        if (del != line_number)

            putc(curr, temp);

            curr = getc(source);
            if (curr == '\n')
            {
                line_number++;
            }
            if (curr == EOF)
            {
                break;

        }
    }
    fclose(source);
    fclose(temp);
    remove(SOURCE_FILE);
    rename(TEMP_FILE, SOURCE_FILE);
}

void InsertSortedModels(model* autos, int number_of_rows)
{
    FILE* source;

    source = fopen(SOURCE_FILE, "w");

    for (int i = 0; i < number_of_rows; i++)
    {
        fprintf(source, "%s %d %d %d %d %d %d",
                autos[i].name,
                autos[i].year,
                autos[i].engine_capacity,
                autos[i].horse_power,
                autos[i].mileage,
                autos[i].tax,
                autos[i].age);
        if (i != number_of_rows-1)
        {
            fprintf(source, "\n");
        }
    }
    fclose(source);
}

void SortedModels(model* autos, int number_of_rows)
{
    int choice = 0;
    model temp;
    printf("----------------------------------------------------------\n");
    printf("Выберите способ сортировки\n");
    printf("----------------------------------------------------------\n");
    printf("1 По году выпуска (возрасту)\n");
    printf("2 По мощности двигателя\n");
    printf("3 По пробегу\n");
    printf("4 По объему двигателя\n");
    printf("5 По стоимости налога\n");

    choice = ValidInput();

    switch(choice)
    {
        case SORT_BY_YEAR:
            for (int i = 0; i < number_of_rows - 1; i++)
            {
                for (int j = 0; j < (number_of_rows - 1 - i); j++)
                {
                    if (autos[j].year < autos[j + 1].year)
                    {
                        temp = autos[j];
                        autos[j] = autos[j + 1];
                        autos[j + 1] = temp;
                    }
                }
            }
            break;

        case SORT_BY_HP:
            for (int i = 0; i < number_of_rows - 1; i++)
            {
                for (int j = 0; j < (number_of_rows - 1 - i); j++)
                {
                    if (autos[j].horse_power < autos[j + 1].horse_power)
                    {
                        temp = autos[j];
                        autos[j] = autos[j + 1];
                        autos[j + 1] = temp;
                    }
                }
            }
            break;

        case SORT_BY_ENGINE_CAPACITY:
            for (int i = 0; i < number_of_rows - 1; i++)
            {
                for (int j = 0; j < (number_of_rows - 1 - i); j++)
                {
                    if (autos[j].engine_capacity < autos[j + 1].engine_capacity)
                    {
                        temp = autos[j];
                        autos[j] = autos[j + 1];
                        autos[j + 1] = temp;
                    }
                }
            }
            break;

        case SORT_BY_MILEAGE:
            for (int i = 0; i < number_of_rows - 1; i++)
            {
                for (int j = 0; j < (number_of_rows - 1 - i); j++)
                {
                    if (autos[j].mileage < autos[j + 1].mileage)
                    {
                        temp = autos[j];
                        autos[j] = autos[j + 1];
                        autos[j + 1] = temp;
                    }
                }
            }
            break;

        case SORT_BY_TAX:
            for (int i = 0; i < number_of_rows - 1; i++)
            {
                for (int j = 0; j < (number_of_rows - 1 - i); j++)
                {
                    if (autos[j].tax < autos[j + 1].tax)
                    {
                        temp = autos[j];
                        autos[j] = autos[j + 1];
                        autos[j + 1] = temp;
                    }
                }
            }
            break;

        default:
            IncorrectNotificator();
            break;
    }


}

void SearchInExistingModels(model* autos, int number_of_rows)
{
    int choice = 0;
    int tmp = 0;
    char* string;

    printf("----------------------------------------------------------\n");
    printf("Выберите параметр поиска\n");
    printf("----------------------------------------------------------\n");
    printf("1 По навзанию марки\n");
    printf("2 По году выпуска\n");
    printf("3 По мощности двигателя\n");
    printf("4 По пробегу\n");

    choice = ValidInput();

    switch(choice)
    {
        case SEARCH_BY_NAME:
            printf("Введите название марки: ");
            string = StringInput();
            printf("Марка   | Год выпуска | Объем дв. | Мощность | Пробег | Налог | Возраст\n");
            printf("----------------------------------------------------------\n");
            for (int i = 0; i < number_of_rows; i++)
            {
                if (strcmp(string, autos[i].name) == 0)
                {
                    PrintOneModel(autos[i]);
                }
            }
            break;
        case SEARCH_BY_YEAR:
            printf("Введите год выпуска: ");
            tmp = ValidInput();
            printf("1 Автомобили %d года выпуска\n", tmp);
            printf("2 Автомобили старше\n");
            printf("3 Автомобили младше\n");

            choice = ValidInput();
            printf("----------------------------------------------------------\n");
            printf("Марка   | Год выпуска | Объем дв. | Мощность | Пробег | Налог | Возраст\n");
            printf("----------------------------------------------------------\n");
            switch(choice)
            {
                case SWITCH_FIRST:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp == autos[i].year)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_SECOND:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp >= autos[i].year)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_THIRD:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp <= autos[i].year)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case SEARCH_BY_HP:
            printf("Введите мощность (лошадиных сил): ");
            tmp = ValidInput();
            printf("1 Автомобили с мощностью двигателя %d сил(ы)\n", tmp);
            printf("2 Автомобили мощнее\n");
            printf("3 Автомобили слабее\n");

            choice = ValidInput();
            printf("----------------------------------------------------------\n");
            printf("Марка   | Год выпуска | Объем дв. | Мощность | Пробег | Налог | Возраст\n");
            printf("----------------------------------------------------------\n");
            switch(choice)
            {
                case SWITCH_FIRST:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp == autos[i].horse_power)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_SECOND:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp <= autos[i].horse_power)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_THIRD:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp >= autos[i].horse_power)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        case SEARCH_BY_MILEAGE:
            printf("Введите пробег (км): ");
            tmp = ValidInput();
            printf("1 Автомобили с пробегом %d км\n", tmp);
            printf("2 Автомобили с меньшим пробегом\n");
            printf("3 Автомобили с бОльшим пробегом\n");

            choice = ValidInput();
            printf("----------------------------------------------------------\n");
            printf("Марка   | Год выпуска | Объем дв. | Мощность | Пробег | Налог | Возраст\n");
            printf("----------------------------------------------------------\n");
            switch(choice)
            {
                case SWITCH_FIRST:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp == autos[i].mileage)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_SECOND:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp >= autos[i].mileage)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                case SWITCH_THIRD:
                    for (int i = 0; i < number_of_rows; i++)
                    {
                        if (tmp <= autos[i].mileage)
                        {
                            PrintOneModel(autos[i]);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

int main()
{
    int number_of_rows = 0;
    int choice = 0;
    model* autos;

    do
    {
        printf("----------------------------------------------------------\n");
        printf("1 Вывести все позиции базы данных\n");
        printf("2 Добавить новую позицию\n");
        printf("3 Удалить позицию\n");
        printf("4 Сортировка по параметру\n");
        printf("5 Поиск по параметрам\n");
        printf("6 Выход\n");

        choice = ValidInput();

        switch(choice)
        {
            case MENU_VIEW_ALL_ROWS:
                number_of_rows = CountNumberOfExistingRows();
                autos = (model*)malloc(number_of_rows * sizeof(model));
                ReadExistingRows(autos);
                printf("----------------------------------------------------------\n");
                PrintExistingRows(autos, number_of_rows);
                free(autos);
                break;

            case MENU_ADD_NEW_ROW:
                AddNewModel();
                printf("----------------------------------------------------------\n");
                printf("Позиция успешно добавлена!\n");
                break;

            case MENU_DELETE_ROW:
                number_of_rows = CountNumberOfExistingRows();
                RemoveSpecificModel(number_of_rows);
                printf("----------------------------------------------------------\n");
                printf("Позиция успешно удалена!\n");
                break;

            case MENU_SORT:
                number_of_rows = CountNumberOfExistingRows();
                autos = (model*)malloc(number_of_rows * sizeof(model));
                ReadExistingRows(autos);
                SortedModels(autos, number_of_rows);
                printf("----------------------------------------------------------\n");
                InsertSortedModels(autos, number_of_rows);
                PrintExistingRows(autos, number_of_rows);
                free(autos);
                break;

            case MENU_SEARCH:
                number_of_rows = CountNumberOfExistingRows();
                autos = (model*)malloc(number_of_rows * sizeof(model));
                ReadExistingRows(autos);
                SearchInExistingModels(autos, number_of_rows);
                free(autos);
                break;

            case MENU_EXIT:
                printf("Goodbye!");
                break;

            default:
                break;
        }


    } while (choice != MENU_EXIT);

    return 0;
}
