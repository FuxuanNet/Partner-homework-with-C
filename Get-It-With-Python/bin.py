'''
文件功能：将图片转化为二值化 txt 文件
使用方法：将 file_in 和 file_out_int 路径匹配到自己要判断的文件路径，运行即可
效果展示：
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000111110000000000000
00000000000011111111100000000000
00000000000111000001110000000000
00000000000110000000011000000000
00000000001100000000001100000000
00000000001000000000001100000000
00000000011000000000000100000000
00000000011000000000000110000000
00000000010000000000000110000000
00000000010000000000000110000000
00000000010000000000000110000000
00000000011000000000000110000000
00000000011000000000000100000000
00000000001000000000001100000000
00000000001100000000001100000000
00000000000110000000011000000000
00000000000111000001110000000000
00000000000001111111100000000000
00000000000000111110000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
00000000000000000000000000000000
'''

import matplotlib.pyplot as plt
from PIL import Image
import os

def handle_int(file_in, width, height, file_out):
    '''
    功能：将图片转化为二值化 txt 文件
    file_in : 输入图片路径
    width ：图片宽度
    height ：图片高度
    file_out : txt文件输出路径
    '''
    image = Image.open(file_in)
    # 缩放处理
    im = image.resize((width, height))
    # im = resized_image.convert('L')
    fh = open(file_out, "w")
    # 从左向右扫描处理
    for i in range(height):
        for j in range(width):
            # 获取像素点(R, G, B)
            c = im.getpixel((j, i))
            # rbg三通道相加
            cs = c[0] + c[1] + c[2]
            # 540是过滤值，可以微调
            if cs < 540:
                fh.write("1")
            else:
                fh.write("0")
        fh.write("\n")
    fh.close()

if __name__ == "__main__":
    '''图片预处理'''
    # 设置图片宽高
    width = 32
    height = 32
    # # 遍历数据集中所有图片
    # for i in range(1, 63):
    #     for j in range(1, 56):
    #         # 读取图片
    #         sam = '%03d' % i
    #         img = '%03d' % j
    #         file_in = f"Img\\Sample{sam}\\img{sam}-{img}.png"
    #         #创建新文件夹并导入转化结果
    #         if not os.path.exists(f"bin\\Sample{sam}"):
    #             os.makedirs(f"bin\\Sample{sam}")
    #         file_out_int = f"bin\\Sample{sam}\\img{sam}-{img}.txt"
    #         handle_int(file_in, width, height, file_out_int)

        #导入判断图片并转化
    file_in = "D:\\vscode\\code\\C_TEST_1\\Partner-homework-with-C\\test\\image_7.png" #自定义上传图片路径 Partner-homework-with-C\test\image_2.png
    file_out_int = "D:\\vscode\\code\\C_TEST_1\\Partner-homework-with-C\\test\\image_7.txt" #自定义保存 txt 文件路径
    handle_int(file_in, width, height, file_out_int)