vs命令工具的使用
1.dumpbin 查看exe引用的动态库,或者查看动态库静态库包含的接口
在需要查看的文件的当前目录中,执行命令    dumpbin -imports aaa.exe (察看导入)      dumpbin -exports bbb.lib(dll)  (察看导出接口)
dumpbin -exports bbb.lib > aa.txt    将信息写入到aa.txt中