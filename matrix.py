'''
代码功能：实现 KNN 算法，用于字符识别。
使用方法：将代码中要判断的 test_file 的文件路径和数据集的路径替换为实际的文件路径，然后运行代码即可。
改进建议：你们可以尝试创建测试集来选取最优的 K 值。
'''

import numpy as np 
from collections import Counter 
#NumPy(Numerical Python) 是 Python 语言的一个扩展程序库，支持大量的维度数组与矩阵运算，此外也针对数组运算提供大量的数学函数库。
#Python 的内置模块 collections 中的 Counter 类来统计列表中元素的频率，并找到出现频率最大的元素。

def read_matrix(file_name):
    '''
    读取 txt 文件并返回矩阵
    file_name: 文件名
    '''
    with open(file_name, 'r') as f: # 打开文件，结束后会自动关闭
        lines = f.readlines() # lines 是一个列表，包含文件中的每一行
    matrix = []
    for line in lines:

        matrix.append([x for x in line.replace('\n', '').replace('\r', '') # 去除换行符
])
    return matrix

def become_vector(matrix):
    '''
    将 32 * 32 矩阵转化为 1 * 1024 向量
    matrix: 32 * 32 矩阵
    '''
    vector = np.array(matrix) # 将列表转化为 numpy 中的 array 的类型
    return vector.flatten().astype(float) # 将字符串转化为浮点数，便于计算
    
def create_label(labels):
    '''
    创建标签
    labels: 标签列表

    > 0 ~ 9 sample 001~010
    > A ~ Z sample 11~036
    > a ~ z sample 37~062
    '''
    for i in range(10):  # 0 到 9
        labels.extend([str(i)] * 55)

    for char in range(65, 91):  # A 到 Z
        labels.extend([chr(char)] * 55)

    for char in range(97, 123):  # a 到 z
        labels.extend([chr(char)] * 55)

def sort_result(distances, labels):
    '''
    对距离和标签进行排序
    distances: 距离列表
    labels: 标签列表
    '''
    # 使用 zip 函数将距离列表和标签列表合并成元组列表
    combined_data = list(zip(distances, labels)) 
    # 根据距离进行排序
    sorted_data = sorted(combined_data, key=lambda x: x[0])
    return sorted_data

def get_top_K(sorted_data,k):
    '''
    获取前五个最小距离的元组
    sorted_data: 排序后的数据
    ''' 

    result_list=sorted_data[:k]
    top_K_labels = [item[1] for item in result_list]
    return top_K_labels

def most_neibor(top_K_labels):
    '''
    统计列表中出现频率最大的元素并输出。
    top_K_labels: 前五个最小距离的标签列表
    '''

    counter = Counter(top_K_labels) # 使用 Counter 类来统计列表中元素的频率

    most_common = counter.most_common(1) # 获取出现频率最大的元素

    return most_common[0][0]

def calculate_distance(vector1, vector2):
    '''
    计算两个向量的欧氏距离
    vector1: 向量1
    vector2: 向量2
    '''
    return np.sqrt(np.sum(np.square(vector1 - vector2))) # 计算欧氏距离并返回

def get_distances(distances,vector1):
    '''
    遍历指定目录下的所有文件

    '''
    for i in range(1, 63):
        for j in range(1, 56):
            sam = '%03d' % i
            img = '%03d' % j
            file = f"bin\\Sample{sam}\\img{sam}-{img}.txt"
            data = read_matrix(file)
            b = become_vector(data)
            distances.append(calculate_distance(vector1, b)) # 计算欧氏距离并写入列表

if __name__ == '__main__':
    labels = [] # 创建标签空列表
    create_label(labels) # 写入标签
    test_file = 'test\\image.txt' #自定义保存 txt 文件路径，可替换
    test_data = read_matrix(test_file) # 读取 txt 文件并返回矩阵
    vector1 = become_vector(test_data) # 将 32 * 32 矩阵转化为 1 * 1024 向量
    distances = [] # 创建距离空列表
    get_distances(distances,vector1) # 遍历指定目录下的所有文件
    sorted_data=sort_result(distances, labels) # 对距离和标签进行排序
    k=3 # KNN 算法的 K 值，可替换
    result=get_top_K(sorted_data,3) # 获取前五个最小距离的元组
    result=most_neibor(result) # 统计列表中出现频率最大的元素
    print(result) # 输出结果
