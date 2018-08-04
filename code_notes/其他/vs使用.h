vs命令工具的使用
1.dumpbin 查看exe引用的动态库,或者查看动态库静态库包含的接口
在需要查看的文件的当前目录中,执行命令    dumpbin -imports aaa.exe (察看导入)      dumpbin -exports bbb.lib(dll)  (察看导出接口)
dumpbin -exports bbb.lib > aa.txt    将信息写入到aa.txt中

2.自定义快捷键方式，工具->选项->环境->键盘->
2.1 在头文件和源文件直接切换：编辑器上下文菜单.代码窗口.切换标题代码文件