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
