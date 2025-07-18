# import time
# import os

# # 获取文件大小
# file_path = "./testfile/sanguoyanyi.txt"
# file_size = os.path.getsize(file_path)
# print("test_file is ： 三国演义：赵云单骑救主")
# print("file size is:", file_size, "byte")

# # 获取当前工作目录
# current_dir = os.getcwd()

# # 构建可执行文件的完整路径
# sm4_acc_path = os.path.join(current_dir, "SM4_aesni", "SM4_acc.exe")
# sm4_basic_path = os.path.join(current_dir, "SM4_basic", "SM4_basic.exe")

# test_plaintext_path=os.path.join(current_dir, "testfile", "sanguoyanyi.txt")
# test_key_path=os.path.join(current_dir, "testfile", "key.txt")
# test_ciper_path=os.path.join(current_dir, "testfile", "enc_sanguoyanyi.txt")
# test_vertify_path=os.path.join(current_dir, "testfile", "vertify_sanguoyanyi.txt")

# print(sm4_acc_path)
# print(sm4_basic_path)
# print(test_plaintext_path)
# print(test_key_path)
# print(test_ciper_path)
# print(test_vertify_path)


# # 检查文件是否存在
# if not os.path.exists(sm4_acc_path):
#     print(f"错误: {sm4_acc_path} 不存在!")
# if not os.path.exists(sm4_basic_path):
#     print(f"错误: {sm4_basic_path} 不存在!")

# print("\n\n===================test aesni SM4:=================")
# # 加密
# start = time.time()
# os.system(f'"{sm4_acc_path}" -E "{test_plaintext_path}" "{test_key_path}" "{test_ciper_path}"')
# end = time.time()
# print("enc cost is :", end-start, "s")
# print("average enc cost is ,", (end-start)/file_size, " per byte.")
# print("can enc", 1/((end-start)/file_size), " byte per s.")

# # 解密
# start = time.time()
# os.system(f'"{sm4_acc_path}" -D "{test_ciper_path}" "{test_key_path}" "{test_vertify_path}')
# end = time.time()
# print("dec cost is :", end-start, "s")
# print("average dec cost is ,", (end-start)/file_size, " per byte.")
# print("can dec", 1/((end-start)/file_size), " byte per s.")

# print("===============test basic SM4:==============")
# # 加密
# start = time.time()
# os.system(f'"{sm4_basic_path}"  -E "{test_plaintext_path}" "{test_key_path}" "{test_ciper_path}"')
# end = time.time()
# print("enc cost is :", end-start, "s")
# print("average enc cost is ,", (end-start)/file_size, " per byte.")
# print("can enc", 1/((end-start)/file_size), " byte per s.")

# # 解密
# start = time.time()
# os.system(f'"{sm4_basic_path}" -D "{test_ciper_path}" "{test_key_path}" "{test_vertify_path}')
# end = time.time()
# print("dec cost is :", end-start, "s")
# print("average dec cost is ,", (end-start)/file_size, " per byte.")
# print("can dec", 1/((end-start)/file_size), " byte per s.")


import time
import os

# 获取文件大小
file_path = "./testfile/sanguoyanyi.txt"
file_size = os.path.getsize(file_path)
print("test_file is ： 三国演义：赵云单骑救主")
print("file size is:", file_size, "byte")

# 获取当前工作目录
current_dir = os.getcwd()
print(f"当前工作目录: {current_dir}")

# 构建可执行文件的完整路径
sm4_acc_path = os.path.join(current_dir, "SM4_aesni", "SM4_acc.exe")
sm4_basic_path = os.path.join(current_dir, "SM4_basic", "SM4_basic.exe")

# 检查文件是否存在
if not os.path.exists(sm4_acc_path):
    print(f"错误: {sm4_acc_path} 不存在!")
if not os.path.exists(sm4_basic_path):
    print(f"错误: {sm4_basic_path} 不存在!")

print("\n\n===================test aesni SM4:=================")
# 加密
start = time.time()
#print(f'{sm4_acc_path} -E {file_path} ./testfile/key_standard.txt ./testfile/enc_sanguoyanyi.txt')
os.system(f'{sm4_acc_path} -E {file_path} ./testfile/key_standard.txt ./testfile/enc_sanguoyanyi.txt')
end = time.time()
print("enc cost is :", end-start, "s")
print("average enc cost is ,", (end-start)/file_size, " per byte.")
print("can enc", 1/((end-start)/file_size), " byte per s.")

# 解密
start = time.time()
os.system(f'{sm4_acc_path} -D ./testfile/enc_sanguoyanyi.txt ./testfile/key_standard.txt ./testfile/vertify_sanguoyanyi.txt')
end = time.time()
print("dec cost is :", end-start, "s")
print("average dec cost is ,", (end-start)/file_size, " per byte.")
print("can dec", 1/((end-start)/file_size), " byte per s.")

print("===============test basic SM4:==============")
# 加密
start = time.time()
os.system(f'{sm4_basic_path} -E {file_path} ./testfile/key_standard.txt ./testfile/enc_sanguoyanyi.txt')
end = time.time()
print("enc cost is :", end-start, "s")
print("average enc cost is ,", (end-start)/file_size, " per byte.")
print("can enc", 1/((end-start)/file_size), " byte per s.")

# 解密
start = time.time()
os.system(f'{sm4_basic_path} -D ./testfile/enc_sanguoyanyi.txt ./testfile/key_standard.txt ./testfile/vertify_sanguoyanyi.txt')
end = time.time()
print("dec cost is :", end-start, "s")
print("average dec cost is ,", (end-start)/file_size, " per byte.")
print("can dec", 1/((end-start)/file_size), " byte per s.")
