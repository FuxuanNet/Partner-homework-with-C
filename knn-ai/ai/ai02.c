#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/*一个手写数字的结构体*/
typedef struct {
    int pixel[1024];
    int number;
} SampleData;

/*一个有label的距离结构体*/
typedef struct {
    float distance;
    int number;
} CalculationData;

/*文件路径+名称*/
static const char* TrainingFilePath = "./my-digit-training.txt";
static const char* TestingFilePath  = "./my-digit-testing.txt";
static const char* CalculationFilePath  = "./my-digit-predict.txt";

/*每个数据集的数字个数*/
#define TRAINING_SIZE 943
#define TRAINING_COUNT 10
#define TEST_SIZE 196
#define CALCULATION_SIZE 9

/*求距离*/
float CalculateDistance(SampleData sample_01, SampleData sample_02) {
    int i, square_sum=0.0;
    for(i=0; i<1024; i++) {
        square_sum += pow(sample_01.pixel[i]-sample_02.pixel[i], 2.0);
    }
    return sqrtf(square_sum);
}

/*把文件的數據加載到內存中*/
int DataLoading(SampleData *pSampleDate_t, FILE *fp, int *pNumber) {
  int index=0;
  for(index = 0; index<1024; index++) {
      if(!fscanf(fp, "%d", &pSampleDate_t->pixel[index])) {
          printf("FILE already read finish.\n");
          return -true;
      }
  }

  fscanf(fp, "%d", &pSampleDate_t->number);
  *pNumber = pSampleDate_t->number;
  return true;
}

/*显示一个Digit 结构体*/
void ShowSampleDate(SampleData SampleData_t) {
  int i, j, id;
  for(i=0;i<32;i++) {
      for(j=0;j<32;j++) {
          printf("%d", SampleData_t.pixel[i*32+j]);
      }
      printf("\n");
  }
  printf(" %d \n", SampleData_t.number);
}

/*交换字符串两项*/
void IndexExchange(CalculationData *in, int index1, int index2) {
  CalculationData tmp = (CalculationData)in[index1];
  in[index1] = in[index2];
  in[index2] = tmp;
}

/*选择排序*/
void SelectSort(CalculationData *in, int length) {
  int i, j, min;
  int N = length;
  for(i=0; i<N-1; i++) {
    min = i;
    for(j=i+1;j<N;j++) {
        if(in[j].distance<in[min].distance) min = j;
    }
    IndexExchange(in,i,min);
  }
}

/*利用训练数据预测一个数据digit*/
int DataForecast(int K, SampleData in, SampleData *train, int nt)
{
  int i, it;
  CalculationData distance[nt];
  /*求取输入digit与训练数据的距离*/
  for(it=0; it<nt; it++) {
    distance[it].distance = CalculateDistance(in, train[it]);
    distance[it].number = train[it].number;
  }
  /*给计算的距离排序(选择排序)*/
  int predict = 0;
  SelectSort(distance, nt);
  for(i=0; i<K; i++) {
    predict += distance[i].number;
  }
  return (int)(predict/K);
}

/*用测试数据集进行测试*/
void KnnDatasetClassification(int K)
{
  printf(".KnnDatasetClassification.\n");

  int i;
  FILE *fp;

  /*读入训练数据*/
  int trainLabels[TRAINING_SIZE];
  int trainCount[TRAINING_COUNT] = {0};
  SampleData *pSampleTrain_t = (SampleData*)malloc(TRAINING_SIZE*sizeof(SampleData));
  fp = fopen(TrainingFilePath,"r");
  if (fp < 0) {
    printf("..Open %s Error.\n", TrainingFilePath);
    exit(0);
  }
  printf("..load training digits.\n");
  for(i=0; i<TRAINING_SIZE; i++) {
    DataLoading(&pSampleTrain_t[i], fp, &trainLabels[i]);
    trainCount[pSampleTrain_t[i].number] ++;
  }
  fclose(fp);
  printf("..Done.\n");

  /*读入测试数据*/
  int testLabels[TEST_SIZE];
  int testCount[TRAINING_COUNT] = {0};
  SampleData *pSampleDataTest_t = (SampleData*)malloc(TEST_SIZE*sizeof(SampleData));
  fp = fopen(TestingFilePath,"r");
  if (fp < 0) {
    printf("..Open %s Error.\n", TestingFilePath);
    exit(0);
  }
  printf("..load testing digits.\n");
  for(i=0;i<TEST_SIZE;i++) {
    DataLoading(&pSampleDataTest_t[i], fp, &testLabels[i]);
    testCount[pSampleDataTest_t[i].number] ++;
  }
  fclose(fp);
  printf("..Done.\n");

  /*求测试数据与训练数据之间的距离*/
  printf("..Cal CalculationData begin.\n");
  CalculationData Distance2Train[TRAINING_SIZE];
  int CorrectCount[TRAINING_COUNT] = {0};
  int itrain, itest, predict;
  for(itest=0; itest < TEST_SIZE; itest++) {
    predict = DataForecast(K, pSampleTrain_t[itest], pSampleTrain_t, TRAINING_SIZE);
    //printf("%d-%d\n",predict, Dtest[itest].number);

    /*给预测准确的进行计数*/
    if(predict == pSampleDataTest_t[itest].number) {
      CorrectCount[predict] ++;
    }
  }

  /*输出测试数据的准确率*/
  printf("    Correct radio:   \n\n");
  for(i=0;i<10;i++) {
    printf("%d:  (  %2d / %2d ) =  %.2f%%\n",
            i,
            CorrectCount[i],
            testCount[i],
            (float)(CorrectCount[i]*1.0/testCount[i]*100));
  }
}
/*预测数据*/
void KnnPredict(int K) {
  int i;
  FILE *fp = NULL;

  /*读入训练数据*/
  int trainLabels[TRAINING_SIZE];
  int trainCount[TRAINING_COUNT] = {0};
  SampleData *Dtrain = (SampleData*)malloc(TRAINING_SIZE*sizeof(SampleData));
  fp = fopen(TrainingFilePath,"r");
  printf("..load training digits.\n");

  for(i=0; i<TRAINING_SIZE; i++) {
      DataLoading(&Dtrain[i], fp, &trainLabels[i]);
      trainCount[Dtrain[i].number] ++;
  }

  fclose(fp);
  printf("..Done.\n");

  /*读入需要预测的数据*/
  int predictLabels[CALCULATION_SIZE];
  int predictCount[TRAINING_COUNT] = {0};
  SampleData *Dpredict = (SampleData*)malloc(CALCULATION_SIZE*sizeof(SampleData));
  fp = fopen(CalculationFilePath,"r");
  printf("..load predict digits.\n");

  for(i=0;i<CALCULATION_SIZE;i++) {
      DataLoading(&Dpredict[i], fp, &predictLabels[i]);
      predictCount[Dpredict[i].number] ++;
  }

  fclose(fp);
  printf("..Done.\n");

  /*求输入数据与训练数据之间的距离*/
  printf("..Cal CalculationData begin.\n");
  CalculationData Distance2Train[TRAINING_SIZE];
  int itrain, ipredict, predict;
  for(ipredict=0; ipredict<CALCULATION_SIZE; ipredict++) {
      predict = DataForecast(K, Dpredict[ipredict], Dtrain, TRAINING_SIZE);
      printf("%d\n",predict);
  }
}


int main(int argc, char** argv)
{
  int K = 1;
  /*对已知数据进行测试，统计预测的正确率*/
  KnnDatasetClassification(K);
  /*对位置数据进行预测*/
  KnnPredict(K);

  return 1;
}