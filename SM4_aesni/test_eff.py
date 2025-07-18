import time
import os
#由于测试写出来的SM4代码性能。
file_size=os.path.getsize("sample.txt")
#enc
start=time.time()
os.system('SM4_acc.exe -E ./sample.txt ./key.txt ./encsample.txt')
end=time.time()
print("enc cost is :",end-start,"s")
print("average enc cost is ,",(end-start)/file_size," per byte.")
print("can enc",1/((end-start)/file_size)," byte per s.")

start=time.time()
os.system('SM4_acc.exe -D ./encsample.txt ./key.txt ./vertify.txt')
end=time.time()
print("dec cost is :",end-start,"s")
print("average dec cost is ,",(end-start)/file_size," per byte.")
print("can dec",1/((end-start)/file_size)," byte per s.")

