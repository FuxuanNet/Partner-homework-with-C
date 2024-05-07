/*
功能：基于 KNN 算法实现英文大小写字母及手写数字的识别。

文件名：main.c

小组成员：李星汉，张福轩，丁政翔

项目开发地址：https://github.com/FuxuanNet/Partner-homework-with-C

合作方式：我们小组采取了 Git + GitHub 合作方式，使用 Git 进行版本控制，使用 GitHub 进行代码托管和协作。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LABELS_Kinds 62 // 包括 26 个小写字母，26个大写字母和 10 个数字

/* 一个手写数字的结构体 */
typedef struct
{
    int pixel[1024]; // 像素数组，存储手写数字图像的像素值
    int label; // 标签，表示手写数字的类别
} Digit_Lerrte;

/* 一个有label的距离结构体 */
typedef struct
{
    long double distance; // 距离
    int label; // 标签
} Distance;

/* 每个数据集的个数 */
int number = 3411; // 数据集的总数
int number_train = 2790; // 训练集数据数
int number_test = 3411; // 测试集数据数
int number_predict = 1; // 预测数据数

/* 函数声明 */

// 哈希函数，将字符映射为索引
int hash(char label);

// 反向哈希函数，将索引映射回字符
char reverseHash(int index);

 // 计算两个手写数字结构体之间的距离
long double calDistance(Digit_Lerrte digit_Lerrte1, Digit_Lerrte digit_Lerrte2);

// 填充标签数组
void fillLabel_pro(char array[], int size);

// 从文件加载手写数字数据
int loadDigit(Digit_Lerrte *digit_Lerrte, FILE *fp, int *labels, char temp_char);

// 显示一个手写数字结构体
void showDigit(Digit_Lerrte digit_Lerrte);

 // 交换函数，用于排序
void exchange(Distance *in, int index1, int index2);

 // 选择排序函数，用于排序
void selectSort(Distance *in, int length);

 // 预测
int prediction(int K, Digit_Lerrte in, Digit_Lerrte *train, int nt);

// 使用测试数据集进行测试分类
void knn_classifiy(int K); 

// 使用训练数据集进行预测
void knn_predict(int K);

// 主函数
int main(void)
{
    int K = 3;//设置 K值

    /* 对已知数据进行测试，统计预测的正确率 */
    knn_classifiy(K);
    // 对位置数据进行预测

    // knn_predict(K);

    return 0;
}

// Digit_Lerrte1 Digit_Lerrte2

// 哈希函数：根据传入的标签字符，将其映射为一个索引值
int hash(char label)
{
    if (label >= '0' && label <= '9')
    {
        return label - '0'; // 数字部分返回 0 到 9
    }
    if (label >= 'A' && label <= 'Z')
    {
        return label - 'A' + 10; // 10个数字之后是大写字母
    }
    if (label >= 'a' && label <= 'z')
    {
        return label - 'a' + 36; // 10个数字+26个大写字母之后是小写字母
    }
    // 非法输入，则返回 -1
    return -1; 
}

// 反向哈希函数：根据传入的索引值，返回相应的标签字符
char reverseHash(int index)
{
    // 根据index返回实际的标签字符
    if (index >= 0 && index <= 9)
    {
        // 数字部分
        return '0' + index;
    }
    if (index >= 10 && index <= 35)
    {
        // 大写字母部分
        return 'A' + (index - 10);
    }
    if (index >= 36 && index <= 61)
    {
        // 小写字母部分
        return 'a' + (index - 36);
    }
    // 不在有效范围内的输入
    return '?';
}

// 求两个图像之间的欧几里德距离。
long double calDistance(Digit_Lerrte Digit_Lerrte1, Digit_Lerrte Digit_Lerrte2)
{
    long double squareSum = 0.0;
    for (int i = 0; i < 1024; i++)
    {
        long double diff = (long double)Digit_Lerrte1.pixel[i] - (long double)Digit_Lerrte2.pixel[i];

        /*
        用浮点数计算更精确。
        之前用int时把1预测成了H，就离谱。修改之后就正确了。
        */
        
        squareSum += diff * diff; // 这样比使用 pow 函数更高效
    }
    return sqrtf(squareSum);
}

// 填充由字符数组构成的标签数组。
void fillLabel_pro(char array[], int size)
{
    int index = 0;
    char current_char;

    for (current_char = '0'; current_char <= '9'; current_char++)
    {
        for (int i = 0; i < 55; i++)
        {
            array[index++] = current_char;
            if (index >= size)
                return;
        }
    }

    for (current_char = 'A'; current_char <= 'Z'; current_char++)
    {
        for (int i = 0; i < 55; i++)
        {
            array[index++] = current_char;
            if (index >= size)
                return;
        }
    }

    for (current_char = 'a'; current_char <= 'z'; current_char++)
    {
        for (int i = 0; i < 55; i++)
        {
            array[index++] = current_char;
            if (index >= size)
                return;
        }
    }
    return;
}

// 从文件中读取数字图像，并进行哈希处理。
int loadDigit(Digit_Lerrte *digit_Lerrte, FILE *fp, int *labels, char temp_char)
{
    int index = 0;
    for (index = 0; index < 1024; index++)
    {
        if (!fscanf(fp, "%d", &(digit_Lerrte->pixel[index])))
        {
            printf("FILE already read finish.\n");
            return -1;
        }
    }

    digit_Lerrte->label = hash(temp_char);
    *labels = digit_Lerrte->label;

    return 1;
}

// 显示一个Digit_Lerrte 结构体
void showDigit(Digit_Lerrte digit_Lerrte)
{
    int i, j;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 32; j++)
        {
            printf("%d", digit_Lerrte.pixel[i * 32 + j]);
        }
        printf("\n");
    }
    printf(" %d \n", digit_Lerrte.label);
}

// 交换数组中的两个项, 便于排序
void exchange(Distance *in, int index1, int index2)
/* 交换字符串两项 */

/*
在 k-近邻算法（k-NN）中，会根据距离将邻居排序，以便选择最近的 k 个邻居。
这个 exchange 函数会被用来帮助排序距离数组。
*/

{
    Distance tmp = (Distance)in[index1]; // 这里应该可以删掉(Distance)，因为 in已经是 Distance 类型
    in[index1] = in[index2];
    in[index2] = tmp;
}

// 选择排序
void selectSort(Distance *in, int length)
{
    int i, j, min;
    int N = length;
    for (i = 0; i < N - 1; i++)
    {
        min = i;
        for (j = i + 1; j < N; j++)
        {
            if (in[j].distance < in[min].distance)
                min = j;
        }
        exchange(in, i, min);
    }
}

// 预测函数，用于对输入数据进行分类预测
int prediction(int K, Digit_Lerrte in, Digit_Lerrte *train, int nt)  // 注意返回值要一致，稍后修改。
{
    Distance *distances = malloc(sizeof(Distance) * nt);  // 为存储距离和标签的结构体数组动态分配内存，nt为训练样本数量
    if (!distances)
    {
        // 处理内存分配失败情况
        exit(EXIT_FAILURE);
    }

    // 计算输入数据与训练数据的距离
    for (int i = 0; i < nt; i++)
    {
        // 计算输入数据与每个训练数据的距离
        distances[i].distance = calDistance(in, train[i]);

        // 存储训练数据的标签
        distances[i].label = train[i].label;
    }

    // 对距离进行排序
    selectSort(distances, nt);

    // 计算最近的 K个邻居的标签计数
    int labelCounts[MAX_LABELS_Kinds] = {0};
    for (int i = 0; i < K; i++)
    {
        int index = distances[i].label;
        if (index != -1)
        {
            labelCounts[index]++; // 统计 K 个邻居中各个标签出现的次数
        }
    }

/*
    for(int i = 0; i < MAX_LABELS_Kinds; i++) {
        printf("%d:%d ", i, labelCounts[i]);
    }
    printf("\n");
*/

    // 找到计数最高的标签
    int maxCount = 0;
    int mostFrequentLabel = 0;
    for (int i = 0; i < MAX_LABELS_Kinds; i++)
    {
        if (labelCounts[i] > maxCount)
        {
            maxCount = labelCounts[i]; // 更新出现次数最大值
            mostFrequentLabel = i; // 更新出现次数最大的标签
        }
    }

    free(distances); // 释放动态分配的内存

    // 根据前面定义的 hash 函数返回实际的标签
    return mostFrequentLabel;
}

// 用测试数据集进行测试
void knn_classifiy(int K)
{
    char arrayLables[3411]; // 用于存储标签数据的数组

    fillLabel_pro(arrayLables, number); // 贴标签
    printf(".knn_classifiy.\n");

    int i;

    // FILE *fp;

    /* 读入训练数据 */
    int trainLabels[number];
    int trainCount[62] = {0};
    Digit_Lerrte *Dtrain = (Digit_Lerrte *)malloc(number * sizeof(Digit_Lerrte)); // 分配内存空间存储训练数据

    // char folderPath[100]; // 存储文件夹路径的字符串

    printf("..load training digits.\n");
    for (int i = 0; i <= 61; i++)
    {
        // 遍历文件夹中的每个文本文件，假设每个文件夹下有 55 个文件
        for (int j = 1; j <= 45; j++)
        {                            
            char filePath_test[100]; // 存储当前文件的路径的字符串
            FILE *fp;

            // 构建当前文件的路径
            sprintf(filePath_test, "../bin/Sample%03d/img%03d-%03d.txt", i + 1, i + 1, j);

            // 打开当前文件
            fp = fopen(filePath_test, "r");

            if (fp == NULL)
            {
                fprintf(stderr, "Unable to open file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }

            // 加载当前文件中的数字数据并更新 trainCount 数组
            if (loadDigit(&Dtrain[i * 55 + j - 1], fp, &trainLabels[i * 55 + j - 1], arrayLables[i * 55 + j - 1]) == -1)
            {
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            trainCount[i]++;
            fclose(fp);
        }
    }
    printf("..Done.\n");

    /*读入测试数据*/
    int testLabels[number_test];
    int testCount[62] = {0};
    Digit_Lerrte *Dtest = (Digit_Lerrte *)malloc(number_test * sizeof(Digit_Lerrte));

    // char folderPath_test[100]; // 存储文件夹路径的字符串

    printf("..load testing digits.\n");
    for (int i = 0; i <= 61; i++)
    {
        // 遍历文件夹中的每个文本文件，假设每个文件夹下有 55 个文件
        for (int j = 46; j <= 55; j++)
        {                            
            char filePath_test[100]; // 存储当前文件的路径的字符串
            FILE *fp;

            // 构建当前文件的路径
            sprintf(filePath_test, "../bin/Sample%03d/img%03d-%03d.txt", i + 1, i + 1, j);

            // 打开当前文件
            fp = fopen(filePath_test, "r");

            if (fp == NULL)
            {
                fprintf(stderr, "Unable to open file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }

            // 加载当前文件中的数字数据并更新 trainCount 数组
            if (loadDigit(&Dtest[i * 55 + j - 1], fp, &testLabels[i * 55 + j - 1], arrayLables[i * 55 + j - 1]) == -1)
            {
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            testCount[i]++;
            fclose(fp);
        }
    }
    printf("..Done.\n"); // 这里 testCount 最后一个是 0，稍后修改

    /* 求测试数据与训练数据之间的距离 */
    printf("..Cal Distance begin.\n");

    // Distance Distance2Train[number ];

    int CorrectCount[62] = {0};
    int itest, predict;
    for (itest = 0; itest < number_test; itest++)
    {
        predict = prediction(K, Dtest[itest], Dtrain, number);

        // printf("%d-%d\n",predict, Dtest[itest].label);

        /* 给预测准确的进行计数 */
        if (predict == Dtest[itest].label)
        {
            CorrectCount[predict]++;  // 预测正确，增加计数
        }
    }

    /* 输出测试数据的准确率 */
    printf("    Correct radio:   \n\n");

    for (i = 0; i < (int)(sizeof(testCount) / sizeof(int)); i++)
    {
        printf("%c:  (  %2d / %2d ) =  %.2lf%%\n",
               reverseHash(i),
               CorrectCount[i],
               testCount[i],
               (long double)(CorrectCount[i] * 1.0 / testCount[i] * 100));
    }
}

// 用训练数据集进行预测
void knn_predict(int K)
/* 预测数据 */
{
    char arrayLables[3411];
    fillLabel_pro(arrayLables, number); // 贴标签
    printf(".knn_predict.\n");
    /* 读入训练数据 */
    int i;
    int trainLabels[number];
    int trainCount[62] = {0};
    Digit_Lerrte *Dtrain = (Digit_Lerrte *)malloc(number * sizeof(Digit_Lerrte));

    // char folderPath[100]; // 存储文件夹路径的字符串

    printf("..load training digits.\n");
    for (int i = 0; i <= 61; i++)
    {
        // 遍历文件夹中的每个文本文件，假设每个文件夹下有 55 个文件
        for (int j = 1; j <= 45; j++)
        {                            
            char filePath_test[100]; // 存储当前文件的路径的字符串
            FILE *fp;

            // 构建当前文件的路径
            sprintf(filePath_test, "../bin/Sample%03d/img%03d-%03d.txt", i + 1, i + 1, j);

            // 打开当前文件
            fp = fopen(filePath_test, "r");

            if (fp == NULL)
            {
                fprintf(stderr, "Unable to open file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            // 加载当前文件中的数字数据并更新 trainCount 数组
            if (loadDigit(&Dtrain[i * 55 + j - 1], fp, &trainLabels[i * 55 + j - 1], arrayLables[i * 55 + j - 1]) == -1)
            {
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            trainCount[i]++;
            fclose(fp);
        }
    }
    printf("..Done.\n");

    /* 读入需要预测的数据 */
    int predictLabels[number_predict];
    int predictCount[62] = {0};
    Digit_Lerrte *Dpredict = (Digit_Lerrte *)malloc(number_predict * sizeof(Digit_Lerrte));
    char filePath_Predict[100];
    FILE *fp;
    sprintf(filePath_Predict, "..\\test\\image.txt");
    fp = fopen(filePath_Predict, "r");
    printf("..load predict digits.\n");
    for (i = 0; i < number_predict; i++)
    {
        loadDigit(&Dpredict[i], fp, &predictLabels[i], arrayLables[i]);
        predictCount[i]++;
    }
    fclose(fp);
    printf("..Done.\n");

    /* 求输入数据与训练数据之间的距离 */
    printf("..Cal Distance begin.\n");

    // Distance Distance2Train[number ];

    int ipredict, predict;
    for (ipredict = 0; ipredict < number_predict; ipredict++)
    {
        predict = prediction(K, Dpredict[ipredict], Dtrain, number);
        char char_predict = reverseHash(predict);
        printf("result:%c\n", char_predict);
    }
}
