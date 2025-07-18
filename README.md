# Project1-SM4实现以及优化

## 简介

**SM4.0**（原名SMS4.0）是[中华人民共和国政府](https://baike.baidu.com/item/%E4%B8%AD%E5%8D%8E%E4%BA%BA%E6%B0%91%E5%85%B1%E5%92%8C%E5%9B%BD%E6%94%BF%E5%BA%9C/10458633?fromModule=lemma_inlink)采用的一种[分组密码](https://baike.baidu.com/item/%E5%88%86%E7%BB%84%E5%AF%86%E7%A0%81/1436163?fromModule=lemma_inlink)标准，由[国家密码管理局](https://baike.baidu.com/item/%E5%9B%BD%E5%AE%B6%E5%AF%86%E7%A0%81%E7%AE%A1%E7%90%86%E5%B1%80/2712999?fromModule=lemma_inlink)于2012年3月21日发布。相关标准为“GM/T 0002-2012《SM4分组密码算法》（原SMS4分组密码算法）”。

**在**[商用密码](https://baike.baidu.com/item/%E5%95%86%E7%94%A8%E5%AF%86%E7%A0%81/10636778?fromModule=lemma_inlink)体系中，SM4主要用于[数据加密](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%8A%A0%E5%AF%86/11048982?fromModule=lemma_inlink)，其算法公开，分组长度与密钥长度均为128bit，[加密算法](https://baike.baidu.com/item/%E5%8A%A0%E5%AF%86%E7%AE%97%E6%B3%95/2816213?fromModule=lemma_inlink)与**密钥扩展算法**都采用32轮非线性迭代结构，[S盒](https://baike.baidu.com/item/S%E7%9B%92/0?fromModule=lemma_inlink)为固定的8比特输入8比特输出。

**SM4.0中的指令长度被提升到大于64K（即64×1024）的水平，这是SM 3.0规格（渲染指令长度允许大于512）的128倍。**

在本项目中，用C语言分别实现了基础的SM4算法，以及使用AESNI指令优化的SM4算法，二者代码分别在路径 `./SM4_basic`、`./SM4_aesni`下，这两个文件加下也存在对应实现方式的详细说明文档：

[·SM4_basic说明文档](./SM4_basic/README.md)

·[SM4_aesni说明文档](./SM4_aesni/README.md) :包含了aesni指令优化的原理、思路以及实现的讲解，强烈建议一看。

## 快速开始

SM4_basic和SM4_aesni均已编译成可以直接运行的程序，分别是 `./SM4_basic/SM4_basic.exe`、 `./SM4_aesni/sm4_acc.exe`。

这两个程序的命令格式均为：

```command
./{SM4_basic.exe|SM4_acc.exe} {-E|-D}	{./inputfile.txt}	{./keyfile.txt}		{./outputfile.txt}
```

在 `./testfile`下存放了测试文件。`./testfile/example_standard.txt`和 `key_standard.txt`两个文件分别是[国密标准SM4文档](32907-2016-gbt-cd-300.pdf)中给出的示例明文和密钥，你可以用十六进制编辑器查看其中具体的内容。

在当前README.md路径下，你可以通过如下命令加解密：

```
#SM4_basic.exe
.\SM4_basic\sm4_basic.exe -E .\testfile\example_standard.txt .\testfile\key_standard.txt ./testfile/enc_example_standard.txt

.\SM4_basic\sm4_basic.exe -D .\testfile\enc_example_standard.txt .\testfile\key_standard.txt .\testfile/vertify_standard.txt

#SM4_acc.exe
.\SM4_aesni\sm4_acc.exe -E .\testfile\example_standard.txt .\testfile\key_standard.txt ./testfile/enc_example_standard.txt

.\SM4_aesni\sm4_acc.exe -D .\testfile\enc_example_standard.txt .\testfile\key_standard.txt .\testfile/vertify_standard.txt
```

加密生成一个 `enc_example_standard.txt`文件，你可以用十六进制编辑器比对是否和[国密标准SM4文档](32907-2016-gbt-cd-300.pdf)中提供的示例相同。同样的，解密会将解密出的明文存储在 `.\testfile/vertify_standard.txt`中，你可以比对该文件和 `.\testfile\example_standard.txt`的内容来验证加解密的正确性。解密过程我没有设计自动去除可能在加密中起到padding作用空字符，所以如果你用文本编辑器查看解密出的明文，或许末尾会跟着几个NULL字符。

## 性能对比

为了体现基础实现的SM4和通过AESNI指令优化的SM4之间性能的差距，我写了一个简陋的py测试脚本（按理来说应该使用更专业的性能测试工具，将时间开销精确到cycle的，但是我硬件基础比较薄弱，也没有时间再去研究更专业的性能测试工具了，所以目前测试的思路就是加解密一个大文本，统计加解密时间了）。该测试脚本路径为 `./test_eff.py`,你可以执行如下命令来观察性能比对（当然你需要首先安装好python）：

```
py test_eff.py
```

不同的设备上可能性能不相同，以下是在我的电脑上的数据：

```
PS E:\VScodeprogram\SM4> py .\test_eff.py
test_file is ： 三国演义：赵云单骑救主
file size is: 3543544 byte
当前工作目录: E:\VScodeprogram\SM4


===================test aesni SM4:=================
enc cost is : 0.3089616298675537 s
average enc cost is , 8.719000804492725e-08  per byte.
can enc 11469204.12582965  byte per s.
dec cost is : 0.2915992736816406 s
average dec cost is , 8.229029290496764e-08  per byte.
can dec 12152101.599089494  byte per s.
===============test basic SM4:==============
enc cost is : 0.5301840305328369 s
average enc cost is , 1.4961971137732083e-07  per byte.
can enc 6683611.342346025  byte per s.
dec cost is : 0.5369055271148682 s
average dec cost is , 1.5151654025316694e-07  per byte.
can dec 6599939.5071265055  byte per s.
PS E:\VScodeprogram\SM4>

PS E:\VScodeprogram\SM4> py .\test_eff.py
test_file is ： 三国演义：赵云单骑救主
file size is: 12048138 byte
当前工作目录: E:\VScodeprogram\SM4


===================test aesni SM4:=================
enc cost is : 0.7218794822692871 s
average enc cost is , 5.991626940771156e-08  per byte.
can enc 16689957.667345932  byte per s.
dec cost is : 0.8569443225860596 s
average dec cost is , 7.112670211663076e-08  per byte.
can dec 14059417.493591076  byte per s.
===============test basic SM4:==============
enc cost is : 1.8513782024383545 s
average enc cost is , 1.5366508936387966e-07  per byte.
can enc 6507658.9883860685  byte per s.
dec cost is : 1.9604403972625732 s
average dec cost is , 1.6271729268560614e-07  per byte.
can dec 6145628.307202405  byte per s.
PS E:\VScodeprogram\SM4>
```

可以看到aesni优化后的效率要比基础的快接近两倍。当文件增大后，优化性能会接近三倍。

值得注意的是，我实现的使用AESNI指令优化的方案只有在处理大文本的时候才会充分显露出性能优势，文本内容大概在693KB二者性能达到平衡：

```
PS E:\VScodeprogram\SM4> py .\test_eff.py
test_file is ： 三国演义：赵云单骑救主
file size is: 708714 byte
当前工作目录: E:\VScodeprogram\SM4


===================test aesni SM4:=================
enc cost is : 0.12800359725952148 s
average enc cost is , 1.8061389680395968e-07  per byte.
can enc 5536672.52462534  byte per s.
dec cost is : 0.12220311164855957 s
average dec cost is , 1.7242937440005358e-07  per byte.
can dec 5799475.892546585  byte per s.
===============test basic SM4:==============
enc cost is : 0.1522386074066162 s
average enc cost is , 2.1480965157541154e-07  per byte.
can enc 4655284.3071343  byte per s.
dec cost is : 0.13847708702087402 s
average dec cost is , 1.953920580387491e-07  per byte.
can dec 5117915.282931742  byte per s.
PS E:\VScodeprogram\SM4>
```

文件内容继续增大aesni的性能优势会显露并逐步放大，最终aesni指令优化的SM4会比基础SM4性能高2~3倍。反之，文本内容在比693KB小的情况下，基础SM4的性能更强一些。读者可以尝试增删测试文件的字符数量进一步测试。
