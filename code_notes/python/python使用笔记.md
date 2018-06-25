## 一些框架用到的模块
1. from flask_script import Manager 为程序添加一个命令行解析器，例如：可用 python app.py shell 在应用程序上下文中运行python shell
2. 数据库相关操作用  
   from flask.ext.sqlalchemy import SQLAlchemy  
   db = SQLAlchemy(app)  
   创建数据库 db.create_all()  
   更新数据库表结构  
   from flask_migrate import Migrate,MigrateCommand  
   migrate = Migrate(app, db)  
   manager.add_command('db', MigrateCommand)  
   python app.py db init	(创建迁移仓库,只需要第一次创建的时候执行)  
   python app.py db migrate -m "initial migration"	(创建迁移脚本)  
   python app.py db upgrade		(更新数据表结构)  
   tips：如果重启服务器，那么之后每次更新数据库结构，都要一起执行上面两条，如果未重启，执行最后一条即可
3. python语法相关  
3.1 Python为装饰器提供了一个很友好的“语法糖”——@，让我们可以很方便的使用装饰器，装饰的原理不做过多阐述，简言之你在一个函数func上加上@decorator_func, 就相当于decorator_func(func)
3.2 小括号-元组(不可更改)，中括号-列表，大括号-键值对或容器

## ipython
run name.py 运行python文件  

## 在github上的html文件，打开html所在页面，在链接前面加上http://htmlpreview.github.com/?可访问该html页面