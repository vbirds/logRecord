
### c语言写的 日志库

#### 简要:
这是用c语言写的 日志库，兼容 `windows`与`linux`，以前一直它记入日志，现在将它开源，欢迎大家给出意见。

#### 说明
程序的接口为 `LR_LOG(const char *file, int line, int level, int status, const char *fmt, ...)`函数，调用此函数，传入参数。
##### 参数说明：
1. `const char *file` : 文件名称
2. `int line`: 行号
3. `int level: 错误级别
	- 0 -- 没有日志
	- 1 -- debug级别
	- 2 -- info级别
	- 3 -- warning级别
	- 4 -- err级别
4. `int status`: 错误码
5. `const char *fmt`：可变参数（用于LOG信息）

#### 用法

1. windows先在c盘 建立`lrlog`目录，linux则在用户目录建立`log`文件夹；
2. 将`logrecord.h`与`logrecord.c`添加至项目，引用头文件`logrecord.h`；
    ```c
    #include "logrecord.h"
    ```
3.调用 `LR_LOG(参数)`函数

#### 测试
`test.c`为测试程序，结果如下图：

![logRecord](http://7xsc1o.com1.z0.glb.clouddn.com/logRecordlog.png)

