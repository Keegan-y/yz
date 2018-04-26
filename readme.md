in main.cpp:
* test is the answer for question one.
* test2 is for question two.

run './build.sh' to compile

测试题是这样的：
```
要求：在72小时之内完成以下两道编程题，并且在linux gcc环境下编译通过。 我们会在linux 下测试完成的程序， 最终结果以及使用的数据结构和算法都在考察范围内。

首先，解压rawdata.tar.gz文件，把所有csv文件放到一个目录下，修改目录名raw_data_csv。

1.	用c++实现以下功能
输入：raw_data_csv文件夹下的rb1705_20161205.csv 
输出：计算2种信号rb_bv_DIFF, rb_bo_5_AVG将结果逐行输出到output.txt。每行的输出格式为：时间戳（空格）信号名称（空格）信号值
20161205 09:30:15.351 rb_bv_DIFF -160
20161205 09:30:15.351 rb_bo_5_AVG -0.1

信号的计算方法说明
rb_bv_DIFF: 
rb表示计算信号的品种名称。在此题中，对应rb1705合约。
每当rb1705的数据有更新时，该时刻rb_bv_DIFF信号的值为 bidvolume1-askvolume1。其中bidvolume1,askvolume1分别代表该时刻买一的委托量和卖一的委托量。
例如，
09:00:00 bidvolume1=10, askvolume1=20
在09:00:00，rb_bv_DIFF=-10

rb_bo_5_AVG:
rb表示计算信号的品种名称。在此题中，对应rb1705合约。
每当rb1705的数据有更新时，该时刻rb_bo_5_AVG的值为当前(askprice1-bidprice1)的值减去在过去5秒内(askprice1-bidprice1)的平均值。
例如，
09:00:00 askprice1=100, bidprice1=99
09:00:01 askprice1=101, bidprice1=99
09:00:02 askprice1=102, bidprice1=101
09:00:03 askprice1=101, bidprice1=100
09:00:04 askprice1=103, bidprice1=101
09:00:05 askprice1=105, bidprice1=103
09:00:06 askprice1=106, bidprice1=105
在09:00:06，rb_bo_5_AVG = (106-105) – ((101-99)+(102-101)+(101-100)+(103-101)+(105-103))/5

2.	批量计算信号：考虑以下的信号变化方式，使程序能适应不同的信号
输入：遍历raw_data_csv文件夹下的所有csv文件。需要计算的信号位于signals.txt文件内， 每行有一个符合命名规则的信号。
输出：计算所有符合命名规则的信号。

信号的命名方式如下:
品种名_信号类型(bv或者bo_x)_计算方式
品种名对应的合约：在此题中，rb对应rb1705合约，ru对应ru1705合约，cu对应cu1703合约，al对应al1702合约。

bv类型中，计算方式RATIO表示信号值为log(bidvolume1/askvolume1)。

bo类型中，bo后的数字x表示回朔的时间长度（单位为秒），计算方式：SHIFT表示信号值为当前(askprice1-bidprice1)减去x秒之前的(askprice1-bidprice1)的值。

```
