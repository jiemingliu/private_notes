# 在windows下使用virtualenv(或者pyvenv)虚拟开发环境
1. 使用pip install virtualenv安装，如果不行，使用python -m pip install virtualenv安装
2. 使用virtualenv [yourName] 安装项目环境
3. 使用 yourName\Scripts\activate 启动虚拟环境
4. 然后安装所需类库方法，跟非虚拟环境一样
5. 退出虚拟环境 deactivate
   
6. pyvenv在python3.4版本以后自带，用法同virtualenv几乎一样
   
7. 批量安装包的方法，将所需要的包和包的版本写在一个 .txt 文件中，启动虚拟环境，运行命令 pip install -r requests.txt

8. 安装离线包方法：  
第一步：在联网电脑上安装需要的库  
第二步：在python安装目录新建packages文件夹，在安装目录python35\Scripts下启动cmd窗口,pip list查看安装的包，准备文件requirements.txt  
然后打包库，执行代码 pip freeze >requirements.txt  
pip install --download c:\python35\packages -r requirements.txt  
第三步：拷贝packages文件夹和requirements.txt,packages文件夹放在C:\python36下，requirements.txt放在C:\Python35\Scripts下  
然后执行 pip install --no-index --find-links=C:\Python35\packages -r requirements.txt (python27中--find-links改为--find-index)



# 使用virtualenv配置不同版本的python的方法
1，前提，virtualenv存在，配置的python的版本已经安装
2，进入需要配置的目录，执行命令 virtualenv -p c:\Python36\python.exe {虚拟环境目录名}

经验之谈
1，如果 pip install package 失败，可以尝试 python -m pip install package