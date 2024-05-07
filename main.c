#include <stdio.h>
#include <math.h>

#define NUM_SAMPLES 62
#define NUM_FILES_PER_SAMPLE 55
#define FILE_PATH_LENGTH 100
#define MATRIX_SIZE 32

void read_matrix_from_file(char *filename, int vector[NUM_SAMPLES * NUM_FILES_PER_SAMPLE][1024])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    int count = 0;
    char c;
    int vector_index = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '0' || c == '1')
        {
            vector[vector_index][count++] = c - '0'; // 转换为整数
            if (count == 1024)
            {
                vector_index++;
                count = 0;
            }
        }
    }
    fclose(file);
}

double calculate_distance(int vector1[1024], int vector2[1024])
{
    double distance = 0;
    for (int i = 0; i < 1024; i++)
    {
        distance += pow(vector1[i] - vector2[i], 2);
    }
    return sqrt(distance);
}

// void fillLabel(char array[], int size)
// {
//     int index = 0;
//     char current_char;

//     for (current_char = '0'; current_char <= '9'; current_char++)
//     {
//         for (int i = 0; i < 55; i++)
//         {
//             array[index++] = current_char;
//             if (index >= size)
//                 return;
//         }
//     }

//     for (current_char = 'A'; current_char <= 'Z'; current_char++)
//     {
//         for (int i = 0; i < 55; i++)
//         {
//             array[index++] = current_char;
//             if (index >= size)
//                 return;
//         }
//     }

//     for (current_char = 'a'; current_char <= 'z'; current_char++)
//     {
//         for (int i = 0; i < 55; i++)
//         {
//             array[index++] = current_char;
//             if (index >= size)
//                 return;
//         }
//     }
// }

void generate_file_paths(char file_paths[NUM_SAMPLES * NUM_FILES_PER_SAMPLE][FILE_PATH_LENGTH])
{
    char file_path_template[100];
    sprintf(file_path_template, "./bin/Sample%%03d/img%%03d-%%03d.txt");

    int file_index = 0;
    for (int i = 1; i <= NUM_SAMPLES; i++)
    {
        for (int j = 1; j <= NUM_FILES_PER_SAMPLE; j++)
        {
            snprintf(file_paths[file_index], FILE_PATH_LENGTH, file_path_template, i, i, j);
            file_index++;
        }
    }
}

// typedef struct
// {
//     char file_path[100];  //路径
//     char label;
//     double distance;
// } DistanceData;

int main(void)
{
    char file_paths[NUM_SAMPLES * NUM_FILES_PER_SAMPLE][FILE_PATH_LENGTH]; 
    // char labels[NUM_SAMPLES * NUM_FILES_PER_SAMPLE]; 
    // int vector[NUM_SAMPLES * NUM_FILES_PER_SAMPLE][1024]; 
    
    // DistanceData distance_data[NUM_SAMPLES * NUM_FILES_PER_SAMPLE]; 

    generate_file_paths(file_paths);

    for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    {
        printf("%s\n", file_paths[i]);
    }
    

    // fillLabel(labels, NUM_SAMPLES * NUM_FILES_PER_SAMPLE);

    // for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    // {
    //     distance_data[i].distance = -1;
    //     distance_data[i].label = '-';
    //     sprintf(distance_data[i].file_path, "%s", "");
    // }

    // for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    // {
    //     read_matrix_from_file(file_paths[i], vector[i]);
    // }

    // for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    // {
    //     for (int j = 0; j < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; j++)
    //     {
    //         if (i != j)
    //         {
    //             distance_data[j].distance = calculate_distance(vector[i], vector[j]);
    //         }
    //     }
    // }

    // for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    // {
    //     int min_index = i;
    //     for (int j = i + 1; j < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; j++)
    //     {
    //         if (distance_data[j].distance < distance_data[min_index].distance)
    //         {
    //             min_index = j;
    //         }
    //     }

    //     if (min_index != i)
    //     {
    //         DistanceData temp = distance_data[i];
    //         distance_data[i] = distance_data[min_index];
    //         distance_data[min_index] = temp;
    //     }
    // }

    // for (int i = 0; i < NUM_SAMPLES * NUM_FILES_PER_SAMPLE; i++)
    // {
    //     printf("File Path: %s\n", distance_data[i].label);
    // }

    return 0;
}