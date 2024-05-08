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
#define WIDTH 32//用于之后检测用户输入的是否是二值化后的文件
#define HEIGHT 32
#define MAX_LABELS_Kinds 62 // 包括 26 个小写字母，26个大写字母和 10 个数字

/* 一个手写数字和字母的结构体 */
typedef struct
{
    int pixel[1024]; // 像素数组，存储手写数字图像的像素值
    int label; // 标签，表示手写数字的类别
} Digit_Lerrte;

/* 一个有label的距离结构体 */
typedef struct
{
    double distance; // 距离
    int label; // 标签
} Distance;

/* 每个数据集的个数 */
int number = 3410; // 数据集的总数
int number_train = 2790; // 训练集数据数
int number_test = 620; // 测试集数据数(从总图片里抠出来的，本来应该是训练集的)
int number_predict = 1; // 预测数据数


/* 函数声明 */
//用于判断用户输入的是否为二值化后的文件
int isBinarized(FILE *file);

// 哈希函数，将字符映射为索引
int hash(char label);

// 反向哈希函数，将索引映射回字符
char reverseHash(int index);

 // 计算两个手写数字结构体之间的距离
double calDistance(Digit_Lerrte digit_Lerrte1, Digit_Lerrte digit_Lerrte2);

// 填充标签数组
void fillLabel_pro(char array[], int size);

// 从文件加载手写数字数据
int load_data(Digit_Lerrte *digit_Lerrte, FILE *fp, int *labels, char temp_char);

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
    //   knn_classifiy(K);
    // 对位置数据进行预测

    knn_predict(K);

    return 0;
}

// Digit_Lerrte1 Digit_Lerrte2

// 检查图像是否已经二值化的函数
int isBinarized(FILE *file) {

    int image[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (fscanf(file, "%1d", &image[i]) != 1  ) {//检测是否能成功读取//%1d确保一次只读取一个数字！
            return 0;
        }
    }
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (image[i] != 0 && image[i] != 1) {//检测是否只有0和1
            return 0; // 非二值化图像
        }
    }
    return 1; // 二值化图像
}

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
double calDistance(Digit_Lerrte Digit_Lerrte1, Digit_Lerrte Digit_Lerrte2)
{
    double squareSum = 0.0;
    for (int i = 0; i < 1024; i++)
    {
        double diff = (double)Digit_Lerrte1.pixel[i] - (double)Digit_Lerrte2.pixel[i];

        /*
        用浮点数计算更精确。
        之前用int时把1预测成了H，就离谱。修改之后就正确了。
        */
        
        squareSum += diff * diff; // 这样比使用 pow 函数更高效（之前用的pow）
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
        for (int i = 0; i < 55; i++)//因为每个文件夹（每个文件夹都是同一种标签的图片）有55张图片（准确来说应该是二值化后的txt文件）
        {
            array[index++] = current_char;//贴标签
            if (index >= size)//这步感觉有些问题，先搁着，等会再看看
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
int load_data(Digit_Lerrte *digit_Lerrte, FILE *fp, int *labels, char temp_char)//这里的temp_char就是标签
{
    int index = 0;
    for (index = 0; index < 1024; index++)
    {
        if (!fscanf(fp, "%d", &(digit_Lerrte->pixel[index])))//读入数据
        {//进行错误处理，感觉没必要
            printf("error\n");
            return -1;
        }
    }

    digit_Lerrte->label = hash(temp_char);//读入用哈希转化为int的标签
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
            printf("%d", digit_Lerrte.pixel[i * 32 + j]);//打印行
        }
        printf("\n");//换行，打印下一列
    }
    char temp_lable = reverseHash(digit_Lerrte.label);//用反哈希转化为char
    printf(" %c \n", temp_lable);//打印标签
}

// 交换数组中的两个项, 便于排序
void exchange(Distance *in, int index1, int index2)
/* 交换字符串两项 */

/*
在 k-近邻算法（k-NN）中，会根据距离将邻居排序，以便选择最近的 k 个邻居。
这个 exchange 函数会被用来帮助排序距离数组。
*/

{
    Distance tmp = in[index1];
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
        min = i;//每次外循环开始时，都将当前未排序部分的第一个元素视为最小的
        for (j = i + 1; j < N; j++)
        {
            if (in[j].distance < in[min].distance)
                min = j;//在内循环中逐个比较，找到最小值后进行交换操作
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
    之前检查错误用于调试的代码:D
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

    // 返回int 类型的标签，最后在knn_predict函数中转化为char了
    return mostFrequentLabel;
}

// 用测试数据集进行测试
void knn_classifiy(int K)
{
    char arrayLables[3411]; // 用于存储标签数据的数组

    fillLabel_pro(arrayLables, number); // 贴标签
    printf("Run the knn_classify function.\n");

    int i;

    /* 读入训练数据 */
    int trainLabels[number_train];
    int trainCount[62] = {0};//trainCount用来统计每种标签的数量
    Digit_Lerrte *Dtrain = (Digit_Lerrte *)malloc(number_train * sizeof(Digit_Lerrte)); // 分配内存空间存储训练数据

    printf("Loading training data ...\n");
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
            if (load_data(&Dtrain[i * 45 + j - 1], fp, &trainLabels[i * 45 + j - 1], arrayLables[i * 55 + j - 1]) == -1)
            {
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            trainCount[i]++;
            fclose(fp);
        }
    }
    printf("Training data loaded.\n");

    /*读入测试数据*/
    int testLabels[number_test];
    int testCount[62] = {0};
    Digit_Lerrte *Dtest = (Digit_Lerrte *)malloc(number_test * sizeof(Digit_Lerrte));

    // char folderPath_test[100]; // 存储文件夹路径的字符串

    printf("Loading test data ...\n");
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
            if (load_data(&Dtest[i * 10 + j -45 - 1], fp, &testLabels[i * 10 + j -45 - 1], arrayLables[i * 55 + j - 1]) == -1)
            {//这里注意i * 10 + j -45 - 1，0的准确率异常的bug终于找到了（喜）
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            testCount[i]++;
            fclose(fp);
        }
    }
    printf("Test data loaded.\n"); // 这里 testCount 最后一个是 0，稍后修改

    /* 求测试数据与训练数据之间的距离 */
    printf("Predicting ...\n");

    // Distance Distance2Train[number ];

    int CorrectCount[62] = {0};
    int itest, predict;
    for (itest = 0; itest < number_test; itest++)
    {
        predict = prediction(K, Dtest[itest], Dtrain, number_train);//调用prediction函数

        // printf("%d-%d\n",predict, Dtest[itest].label);//用于调试的语句

        /* 给预测准确的进行计数 */
        if (predict == Dtest[itest].label)
        {
            CorrectCount[predict]++;  // 预测正确，增加计数
        }
    }
    printf("Prediction completed.\n");
    /* 输出测试数据的准确率 */
    printf("Accuracy :\n");

    for (i = 0; i < (int)(sizeof(testCount) / sizeof(int)); i++)
    {
        printf("%c: ( %2d / %2d ) =  %.2lf%%\n",
               reverseHash(i),//将int标签转化为char标签
               CorrectCount[i],
               testCount[i],
               (double)(CorrectCount[i] * 1.0 / testCount[i] * 100));
    }
    free(Dtrain);
    free(Dtest);

}

// 用训练数据集进行预测
void knn_predict(int K)
/* 预测数据 */
{
    char arrayLables[3411];
    fillLabel_pro(arrayLables, number); // 贴标签
    printf("Run the knn_predict function.\n");
    /* 读入训练数据 */
    //以下和上面的函数相同
    int i;
    int trainLabels[number];
    int trainCount[62] = {0};
    Digit_Lerrte *Dtrain = (Digit_Lerrte *)malloc(number * sizeof(Digit_Lerrte));

    // char folderPath[100]; // 存储文件夹路径的字符串

    printf("Loading training data ...\n");
    for (int i = 0; i <= 61; i++)
    {
        // 遍历文件夹中的每个文本文件，假设每个文件夹下有 55 个文件
        for (int j = 1; j <= 55; j++)
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
            if (load_data(&Dtrain[i * 55 + j - 1], fp, &trainLabels[i * 55 + j - 1], arrayLables[i * 55 + j - 1]) == -1)
            {
                // 处理加载失败情况
                fprintf(stderr, "Failed to load data from file: %s\n", filePath_test);
                exit(EXIT_FAILURE);
            }
            trainCount[i]++;
            fclose(fp);
        }
    }
    printf("Training data loaded.\n");

    /* 读入需要预测的数据 */
    
    int predictLabels[number_predict];
    int predictCount[62] = {0};
    Digit_Lerrte *Dpredict = (Digit_Lerrte *)malloc(number_predict * sizeof(Digit_Lerrte));
    char filePath_Predict[100];
    FILE *fp;
    char path[1024];
    printf("Please enter the absolute file path (binary txt 32*32) ...\n");
    scanf("%1024s",path);
    sprintf(filePath_Predict, path);// 构建当前文件的路径
    fp = fopen(filePath_Predict, "r");
    int COUNT = 0;//当循环五次就停止，安全
    while(!isBinarized(fp) && COUNT < 5){//判断用户输入是否正确
        printf("File don't exist or The image has not been binarized!\n");
        scanf("%1024s",path);
        sprintf(filePath_Predict, path);
        fp = fopen(filePath_Predict, "r");
        COUNT++;
    }

    printf("Loading input data ...\n");
    for (i = 0; i < number_predict; i++)
    {
        load_data(&Dpredict[i], fp, &predictLabels[i], arrayLables[i]);
        predictCount[i]++;
    }
    fclose(fp);
    printf("Input data loaded.\n");

    /* 求输入数据与训练数据之间的距离 */
    printf("Predicting ...\n");

    //这个也是调试时的代码 Distance Distance2Train[number ];

    int ipredict, predict;
    for (ipredict = 0; ipredict < number_predict; ipredict++)
    {
        predict = prediction(K, Dpredict[ipredict], Dtrain, number);
        char char_predict = reverseHash(predict);//转化为char类型标签
        printf("Result:%c\n", char_predict);
    }
    fclose(fp);
    free(Dtrain);
    free(Dpredict);
}
/*
在控制台打印中文就会乱码，实测如下，笑死，老老实实改成英文吧
杩愯knn_classifiy鍑芥暟
鍔犺浇璁粌鏁版嵁涓?..
璁粌鏁版嵁鍔犺浇瀹屾垚
鍔犺浇娴嬭瘯鏁版嵁涓?..
娴嬭瘯鏁版嵁鍔犺浇瀹屾垚
棰勬祴涓?..
*/
/*
理论上用第二个函数预测的准确率要比第一个函数算出来的准确率高（虽然还是依托），
因为对于每种标签第一个函数训练了number_train = 2790张图片（其中有number_test = 620张图片被拿去算准确率了），
而第二个函数训练了number = 3410张图片
*/


