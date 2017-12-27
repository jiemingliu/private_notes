Visual C++ 2012 使用了更加安全的 run-time library routines 。新的Security CRT functions（就是那些带有“_s”后缀的函数）
方法一：将原来的旧函数替换成新的 Security CRT functions。

方法二：用以下方法屏蔽这个警告：

    1. 在预编译头文件stdafx.h里（注意：一定要在没有include任何头文件之前）定义下面的宏：

       #define _CRT_SECURE_NO_DEPRECATE

    2. 或声明 #pragma warning(disable:4996)

    3. 更改预处理定义：

        项目->属性->配置属性->C/C++ -> 预处理器 -> 预处理器定义，增加：

            _CRT_SECURE_NO_DEPRECATE

方法三：方法二没有使用更加安全的 CRT 函数，显然不是一个值得推荐的好方法，但我们又不想一个一个地改函数名，这里还有一个更简便的方法：

在预编译头文件 stdafx.h 里（同样要在没有include任何头文件之前）定义下面的宏：

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

在链接的时候便会自动将旧函数替换成 Security CRT functions 。

注意：这个方法虽然使用了新的函数，但是不能消除警告(原因见红字)，你还得同时使用方法二(-_-)。即实际应在预编译头文件 stdafx.h 里加入下面两句：

#define _CRT_SECURE_NO_DEPRECATE

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1