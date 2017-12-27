1.    面向对象的程序设计思想是什么?

答：把数据结构和对数据结构进行操作的方法封装形成一个个的对象。

 

2.    什么是类?

答：把一些具有共性的对象归类后形成一个集合，也就是所谓的类。



3.    对象都具有的两方面特征是什么?分别是什么含义?

答：对象都具有的特征是：静态特征和动态特征。

静态特征是指能描述对象的一些属性（成员变量），动态特征是指对象表现出来的行为（成员函数）

 

4.    在头文件中进行类的声明，在对应的实现文件中进行类的定义有什么意义?

答：这样可以提高编译效率，因为分开的话只需要编译一次生成对应的.obj文件后，再次应用该类的地方，这个类就不会被再次编译，从而大大的提高了编译效率。

 

5.    在类的内部定义成员函数的函数体，这种函数会具备那种属性?

答：这种函数会自动为内联函数，这种函数在函数调用的地方在编译阶段都会进行代码替换。

 

6.    成员函数通过什么来区分不同对象的成员数据?为什么它能够区分?

答：通过this指针指向对象的首地址来区分的。

 

7.    C++编译器自动为类产生的四个缺省函数是什么?

答：默认构造函数，拷贝构造函数，析构函数，赋值函数。

 

8.    拷贝构造函数在哪几种情况下会被调用?

答：

1.当类的一个对象去初始化该类的另一个对象时；

2.如果函数的形参是类的对象，调用函数进行形参和实参结合时；

3.如果函数的返回值是类对象，函数调用完成返回时。

 

9.    构造函数与普通函数相比在形式上有什么不同?（构造函数的作用，它的声明形式来分析）

答：构造函数是类的一种特殊成员函数，一般情况下，它是专门用来初始化对象成员变量的。

构造函数的名字必须与类名相同，它不具有任何类型，不返回任何值。

 

10.  什么时候必须重写拷贝构造函数?

答：当构造函数涉及到动态存储分配空间时，要自己写拷贝构造函数，并且要深拷贝。

 

11.  构造函数的调用顺序是什么?

答：1.先调用基类构造函数

2.按声明顺序初始化数据成员

3.最后调用自己的构造函数。

 

12.  哪几种情况必须用到初始化成员列表?

答：类的成员是常量成员初始化；

类的成员是对象成员初始化，而该对象没有无参构造函数。

类的成员为引用时。

 

13.  什么是常对象?

答：常对象是指在任何场合都不能对其成员的值进行修改的对象。

 

14.  静态函数存在的意义?

答：静态私有成员在类外不能被访问，可通过类的静态成员函数来访问；

当类的构造函数是私有的时，不像普通类那样实例化自己，只能通过静态成员函数来调用构造函数。

 

15.  在类外有什么办法可以访问类的非公有成员?

答：友元，继承，公有成员函数。

 

16.  什么叫抽象类?

答：不用来定义对象而只作为一种基本类型用作继承的类。

 

17.  运算符重载的意义?

答：为了对用户自定义数据类型的数据的操作与内定义的数据类型的数据的操作形式一致。

 

18.  不允许重载的5个运算符是哪些?

答：

1.    .*（成员指针访问运算符号）

2.    ：：域运算符

3.    Sizeof 长度运算符号

4.    ?：条件运算符号

5.  .（成员访问符）

 

19.  运算符重载的三种方式?

答：普通函数，友元函数，类成员函数。

 

20.  流运算符为什么不能通过类的成员函数重载?一般怎么解决?

答：因为通过类的成员函数重载必须是运算符的第一个是自己，而对流运算的重载要求第一个参数是流对象。所以一般通过友元来解决。

 

21.  赋值运算符和拷贝构造函数的区别与联系?

答：相同点：都是将一个对象copy到另一个中去。

不同点：拷贝构造函数涉及到要新建立一个对象。

 

22.  在哪种情况下要调用该类的析构函数?

答：对象生命周期结束时。

 

23.  对象间是怎样实现数据的共享的?

答：通过类的静态成员变量来实现对象间的数据共享。静态成员变量占有自己独立的空间不为某个对象所私有。

 

24.  友元关系有什么特性?

答：单向的，非传递的，不能继承的。

 

25.  对对象成员进行初始化的次序是什么?

答：它的次序完全不受它们在初始化表中次序的影响，只由成员对象在类中声明的次序来决定的。

 

26.  类和对象之间的关系是什么?

答：类是对象的抽象，对象是类的实例。

 

27.  对类的成员的访问属性有什么?

答：public，protected，private。

 

28．const char *p和char *  const p; 的区别

答：

如果const位于星号的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；

如果const位于星号的右侧，const就是修饰指针本身，即指针本身是常量。

 

29.  是不是一个父类写了一个virtual 函数，如果子类覆盖它的函数不加virtual ,也能实现多态?

答：

virtual修饰符会被隐形继承的。

virtual可加可不加,子类覆盖它的函数不加virtual ,也能实现多态。

 

30.  函数重载是什么意思?它与虚函数的概念有什么区别?

答：函数重载是一个同名函数完成不同的功能，编译系统在编译阶段通过函数参数个数、参数类型不同，函数的返回值来区分该调用哪一个函数，即实现的是静态的多态性。但是记住：不能仅仅通过函数返回值不同来实现函数重载。而虚函数实现的是在基类中通过使用关键字virtual来申明一个函数为虚函数，含义就是该函数的功能可能在将来的派生类中定义或者在基类的基础之上进行扩展，系统只能在运行阶段才能动态决定该调用哪一个函数，所以实现的是动态的多态性。它体现的是一个纵向的概念，也即在基类和派生类间实现。

 

31.  构造函数和析构函数是否可以被重载,为什么?

答：构造函数可以被重载，析构函数不可以被重载。因为构造函数可以有多个且可以带参数，而析构函数只能有一个，且不能带参数。

 

32.  如何定义和实现一个类的成员函数为回调函数?

答：

所谓的回调函数，就是预先在系统对函数进行注册，让系统知道这个函数的存在，以后，当某个事件发生时，再调用这个函数对事件进行响应。

定义一个类的成员函数时在该函数前加CALLBACK即将其定义为回调函数，函数的实现和普通成员函数没有区别

 

33.  虚函数是怎么实现的?

答：简单说来使用了虚函数表.

 

34.  抽象类不会产生实例，所以不需要有构造函数。 错

 

35.  从一个模板类可以派生新的模板类，也可以派生非模板类。 对

 

36.  main 函数执行以前，还会执行什么代码?

答案：全局对象的构造函数会在main 函数之前执行。

 

37.  当一个类A 中没有申明任何成员变量与成员函数,这时sizeof(A)的值是多少，如果不是零，请解释一下编译器为什么没有让它为零。（Autodesk）

答案：肯定不是零。举个反例，如果是零的话，声明一个class A[10]对象数组，而每一个对象占用的空间是零，这时就没办法区分A[0],A[1]…了。

 

38.  delete与 delete []区别：

答：delete只会调用一次析构函数，而delete[]会调用每一个成员的析构函数。

 

39．子类析构时要调用父类的析构函数吗?

答：会调用。析构函数调用的次序是先派生类的析构后基类的析构，也就是说在基类的的析构调用的时候,派生类的信息已经全部销毁了

 

40.  继承的优缺点。

1、类继承是在编译时刻静态定义的，且可直接使用，

2、类继承可以较方便地改变父类的实现。

缺点：

1、因为继承在编译时刻就定义了，所以无法在运行时刻改变从父类继承的实现

2、父类通常至少定义了子类的部分行为，父类的任何改变都可能影响子类的行为

3、如果继承下来的实现不适合解决新的问题，则父类必须重写或被其他更适合的类替换。这种依赖关系限制了灵活性并最终限制了复用性。

 

41.  解释堆和栈的区别。 

答：栈区（stack）— 由编译器自动分配释放 ，存放函数的参数值，局部变量的值等。

堆（heap）一般由程序员分配释放， 若程序员不释放，程序结束时可能由OS回收 。

 

42.  一个类的构造函数和析构函数什么时候被调用,是否需要手工调用?

答：构造函数在创建类对象的时候被自动调用，析构函数在类对象生命期结束时，由系统自动调用。

 

43.  何时需要预编译：

答：总是使用不经常改动的大型代码体。

程序由多个模块组成，所有模块都使用一组标准的包含文件和相同的编译选项。在这种情况下，可以将所有包含文件预编译为一个预编译头。

 

44.  多态的作用?

答：主要是两个：

1. 隐藏实现细节，使得代码能够模块化；扩展代码模块，实现代码重用；

2. 接口重用：为了类在继承和派生的时候，保证使用家族中任一类的实例的某一属性时的正确调用

 

45.  虚拟函数与普通成员函数的区别?内联函数和构造函数能否为虚拟函数?

答案：区别：虚拟函数有virtual关键字，有虚拟指针和虚函数表，虚拟指针就是虚拟函数的接口，而普通成员函数没有。内联函数和构造函数不能为虚拟函数。

 

46.  构造函数和析构函数的调用顺序? 析构函数为什么要虚拟?

答案：构造函数的调用顺序：基类构造函数—对象成员构造函数—派生类构造函数；析构函数的调用顺序与构造函数相反。析构函数虚拟是为了防止析构不彻底，造成内存的泄漏。

 

47. C++中类型为private的成员变量可以由哪些函数访问?

答：只可以由本类中的成员函数和友元函数访问

 

48.  请说出类中private，protect，public三种访问限制类型的区别

答：private是私有类型，只有本类中的成员函数访问;protect是保护型的，本类和继承类可以访问;public是公有类型，任何类都可以访问.

 

49.  类中成员变量怎么进行初始化?

答：可以通过构造函数的初始化列表或构造函数的函数体实现。

 

50.  在什么时候需要使用“常引用”? 

答：如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用常引用。



51.  引用与指针有什么区别?

答 、1) 引用必须被初始化，指针不必。

2) 引用初始化以后不能被改变，指针可以改变所指的对象。

3) 不存在指向空值的引用，但是存在指向空值的指针。

 

52.  描述实时系统的基本特性

答 、在特定时间内完成特定的任务，实时性与可靠性。

 

54.  全局变量和局部变量在内存中是否有区别?如果有，是什么区别?

答 、全局变量储存在静态数据区，局部变量在堆栈中。

 

55.  堆栈溢出一般是由什么原因导致的?

答 、没有回收垃圾资源

 

56.  什么函数不能声明为虚函数?

答 构造函数（constructor）和内联函数

 

57.  IP地址的编码分为哪俩部分?

答 IP地址由两部分组成，网络号和主机号。

 

58.  不能做switch()的参数类型是：

答 、switch的参数不能为实型。

 

59.  如何引用一个已经定义过的全局变量?

答 、可以用引用头文件的方式，也可以用extern关键字，如果用引用头文件方式来引用某个在头文件中声明的全局变理，假定你将那个变量写错了，那么在编译期间会报错，如果你用extern方式引用时，假定你犯了同样的错误，那么在编译期间不会报错，而在连接期间报错

 

60.  对于一个频繁使用的短小函数,在C语言中应用什么实现,在C++中应用什么实现?

答 、c用宏定义，c++用inline

61.  C++是不是类型安全的?

答案：不是。两个不同类型的指针之间可以强制转换（用reinterpret cast)

 

63.  简述数组与指针的区别?

答：数组要么在静态存储区被创建（如全局数组），要么在栈上被创建。指针可以随时指向任意类型的内存块。

(1)修改内容上的区别

char a[] = “hello”;

a[0] = ‘X’;

char *p = “world”; // 注意p 指向常量字符串

p[0] = ‘X’; // 编译器不能发现该错误，运行时错误

(2) 用运算符sizeof 可以计算出数组的容量（字节数）。sizeof(p),p 为指针得到的是一个指针变量的字节数，而不是p 所指的内存容量。

 

64.  C++函数中值的传递方式

答：有三种方式：值传递、指针传递、引用传递

 

65.  内存的分配方式

答：分配方式有三种，

1、 静态存储区，是在程序编译时就已经分配好的，在整个运行期间都存在，如全局变量、常量。

2、 栈上分配，函数内的局部变量就是从这分配的，但分配的内存容易有限。

3、 堆上分配，也称动态分配，如我们用new,malloc分配内存，用delete,free来释放的内存。

 

66.  extern“C”有什么作用?

答：Extern “C”是由c++提供的一个连接交换指定符号，用于告诉c++这段代码是Ｃ函数。这是因为C++编译后库中函数名会变得很长，与C生成的不一致，造成c++不能直接调用C函数，加上extren “c”后，C++就能直接调用C函数了。

Extern “C”主要使用正规DLL函数的引用和导出 和 在C++包含C函数或C头文件时使用。使用时在前面加上extern “c” 关键字即可。可以用一句话概括extern “C”这个声明的真实目的：实现C++与C及其它语言的混合编程。

 

 

67.  用什么函数开启新进程、线程。

答案：

线程：CreateThread/AfxBeginThread等

进程：CreateProcess等

 

68.  SendMessage和PostMessage有什么区别

答案：SendMessage是阻塞的，等消息被处理后，代码才能走到SendMessage的下一行。PostMessage是非阻塞的，不管消息是否已被处理，代码马上走到PostMessage的下一行。

 

69.  CMemoryState主要功能是什么

答案：查看内存使用情况，解决内存泄露问题。

 

70.  #include 和 #include “filename.h” 有什么区别?

答：对于#include ，编译器从标准库路径开始搜索 filename.h

对于#include “filename.h” ，编译器从用户的工作路径开始搜索 filename.h

 

71.  处理器标识#error的目的是什么?

答：编译时输出一条错误信息，并中止继续编译。

 

72.  #if!defined(AFX_…_HADE_H)

#define(AFX_…_HADE_H)

……

#endif作用?

答：防止该头文件被重复引用。

 

73. 在定义一个宏的时候要注意什么?

答：定义部分的每个形参和整个表达式都必须用括号括起来，以避免不可预料的错误发生

 

74.  数组在做函数实参的时候会转变为什么类型?

答：数组在做实参时会变成指针类型。

 

75.  系统会自动打开和关闭的3个标准的文件是?

(1)   标准输入—-键盘—stdin

(2)   标准输出—-显示器—stdout

(3)   标准出错输出—-显示器—stderr

 

76.  .在Win32下 char, int, float, double各占多少位?

(1)   Char       占用8位

(2)   Int      占用32位

(3)   Float    占用32位

(4)   Double   占用64位

 

77.  strcpy()和memcpy()的区别?

答：strcpy()和memcpy()都可以用来拷贝字符串，strcpy()拷贝以’\0’结束，但memcpy()必须指定拷贝的长度。

 

78.  说明define和const在语法和含义上有什么不同?

答：(1)   #define是C语法中定义符号变量的方法，符号常量只是用来表达一个值，在编译阶段符号就被值替换了，它没有类型；

(2)   Const是C++语法中定义常变量的方法，常变量具有变量特性，它具有类型，内存中存在以它命名的存储单元，可以用sizeof测出长度。

 

79.  说出字符常量和字符串常量的区别，并使用运算符sizeof计算有什么不用?

答：字符常量是指单个字符，字符串常量以‘\0’结束，使用运算符sizeof计算多占一字节的存储空间。

 

80.  简述全局变量的优缺点?

答：全局变量也称为外部变量，它是在函数外部定义的变量，它属于一个源程序文件，它保存上一次被修改后的值，便于数据共享，但不方便管理，易引起意想不到的错误。

 

81.  总结static的应用和作用?

答：（1）函数体内static变量的作用范围为该函数体，不同于auto变量，该变量的内存只被分配一次，因此其值在下次调用时仍维持上次的值；

（2）在模块内的static全局变量可以被模块内所用函数访问，但不能被模块外其它函数访问；

（3）在模块内的static函数只可被这一模块内的其它函数调用，这个函数的使用范围被限制在声明它的模块内；

（4）在类中的static成员变量属于整个类所拥有，对类的所有对象只有一份拷贝；

（5）在类中的static成员函数属于整个类所拥有，这个函数不接收this指针，因而只能访问类的static成员变量。

 

82.  总结const的应用和作用?

答：（1）欲阻止一个变量被改变，可以使用const关键字。在定义该const变量时，通常需要对它进行初始化，因为以后就没有机会再去改变它了；

（2）对指针来说，可以指定指针本身为const，也可以指定指针所指的数据为const，或二者同时指定为const；

（3）在一个函数声明中，const可以修饰形参，表明它是一个输入参数，在函数内部不能改变其值；

（4）对于类的成员函数，若指定其为const类型，则表明其是一个常函数，不能修改类的成员变量；

（5）对于类的成员函数，有时候必须指定其返回值为const类型，以使得其返回值不为“左值”。

 

83.  什么是指针?谈谈你对指针的理解?

答：指针是一个变量，该变量专门存放内存地址；

指针变量的类型取决于其指向的数据类型，在所指数据类型前加*

指针变量的特点是它可以访问所指向的内存。

 

84.  什么是常指针，什么是指向常变量的指针?

答：常指针的含义是该指针所指向的地址不能变，但该地址所指向的内容可以变化，使用常指针可以保证我们的指针不能指向其它的变量，

指向常变量的指针是指该指针的变量本身的地址可以变化，可以指向其它的变量，但是它所指的内容不可以被修改。

 

85.  函数指针和指针函数的区别?

答：函数指针是指向一个函数入口的指针；

指针函数是函数的返回值是一个指针类型。

 

87.  简述Debug版本和Release版本的区别?

答：Debug版本是调试版本，Release版本是发布给用户的最终非调试的版本，

 

88.  指针的几种典型应用情况?

答：

int *p[n];—–指针数组，每个元素均为指向整型数据的指针。

int (*)p[n];—p为指向一维数组的指针，这个一维数组有n个整型数据。

int *p();——函数带回指针，指针指向返回的值。

int (*)p();—-p为指向函数的指针。

 

89.  static函数与普通函数有什么区别?

答：static函数在内存中只有一份，普通函数在每个被调用中维持一份拷贝

 

90.  struct(结构) 和 union(联合)的区别?

答：1. 结构和联合都是由多个不同的数据类型成员组成, 但在任何同一时刻, 联合中只存放了一个被选中的成员（所有成员共用一块地址空间）, 而结构的所有成员都存在（不同成员的存放地址不同）。

2. 对于联合的不同成员赋值, 将会对其它成员重写,  原来成员的值就不存在了, 而对于结构的不同成员赋值是互不影响的。

 

91.  class 和 struct 的区别?

答：struct 的成员默认是公有的，而类的成员默认是私有的。

 

92.  简述枚举类型?

答：枚举方便一次定义一组常量，使用起来很方便；

 

93.  assert()的作用?

答：ASSERT()是一个调试程序时经常使用的宏，在程序运行时它计算括号内的表达式，如果表达式为FALSE (0), 程序将报告错误，并终止执行。如果表达式不为0，则继续执行后面的语句。这个宏通常原来判断程序中是否出现了明显非法的数据，如果出现了终止程序以免导致严重后果，同时也便于查找错误。

 

94.  局部变量和全局变量是否可以同名?

答：能。局部会屏蔽全局。要用全局变量，需要使用”::”(域运算符)。

 

95.  程序的局部变量存在于（堆栈）中，全局变量存在于（静态区 ）中，动态申请数据存在于（ 堆）中。

 

96.  在什么时候使用常引用?

答：如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用常引用。

 

97.  类的声明和实现的分开的好处?

答：1.    起保护作用；

2.    提高编译的效率。

 

98.  windows消息系统由哪几部分构成?

答：由一下3部分组成：

1.    消息队列：操作系统负责为进程维护一个消息队列，程序运行时不断从该消息队列中获取消息、处理消息；

2.    消息循环：应用程序通过消息循环不断获取消息、处理消息。

3.    消息处理：消息循环负责将消息派发到相关的窗口上使用关联的窗口过程函数进行处理。

 

99.  什么是消息映射?

答：消息映射就是让程序员指定MFC类（有消息处理能力的类）处理某个消息。然后由程序员完成对该处理函数的编写，以实现消息处理功能。

 

100. 什么是UDP和TCP的区别是什么?

答：TCP的全称为传输控制协议。这种协议可以提供面向连接的、可靠的、点到点的通信。

UDP全称为用户报文协议，它可以提供非连接的不可靠的点到多点的通信。用TCP还是UDP，那要看你的程序注重哪一个方面?可靠还是快速?


101. winsock建立连接的主要实现步骤?

答：

服务器端：socket()建立套接字，绑定（bind）并监听（listen），用accept（）等待客户端连接, accept（）发现有客户端连接，建立一个新的套接字，自身重新开始等待连接。该新产生的套接字使用send()和recv（）写读数据，直至数据交换完毕，closesocket()关闭套接字。

客户端：socket()建立套接字，连接（connect）服务器，连接上后使用send()和recv（），在套接字上写读数据，直至数据交换完毕，closesocket()关闭套接字。

 

102. 进程间主要的通讯方式?

答：信号量，管道，消息，共享内存

 

103. 构成Win32 API 函数的三个动态链接库是什么?

答：内核库，用户界面管理库，图形设备界面库。

 

104. 创建一个窗口的步骤是?

答：填充一个窗口类结构->注册这个窗口类->然后再创建窗口->显示窗口->更新窗口。

 

105. 模态对话框和非模态对话框有什么区别?

答：1.调用规则不同：前者是用DoModal()调用，后者通过属性和ShowWindow()来显示。

2．模态对话框在没有关闭前用户不能进行其他操作，而非模态对话框可以。

3．非模态对话框创建时必须编写自己的共有构造函数，还要调用Create()函数。

 

106. 从EDIT框中取出数据给关联的变量，已经把关联的变量的数据显示在EDIT框上的函数是什么?

答： UpdateData(TRUE),  Updatedata(FALSE).

 

107. 简单介绍GDI?

答： GDI是Graphics Device Interface 的缩写，译为：图形设备接口；是一个在Windows应用程序中执行与设备无关的函数库，这些函数在不同的输出设备上产生图形以及文字输出。

 

108. windows消息分为几类?并对各类做简单描述。

答：

1.窗口消息：与窗口相关的消息，除WM_COMMAND之外的所有以WM_开头的消息；

2.命令消息；用于处理用户请求，以WM_COMMAND表示的消息；

3.控件通知消息：统一由WM_NOTIFT表示，

4.用户自定义消息。

 

109. 如何自定义消息?

答：使用WM_USER 和WM_APP两个宏来自定义消息，

 

110. 简述Visual C++ 、Win32 API和MFC之间的关系?

答：(1)   Visual C+是一个以C++程序设计语言为基础的、集成的、可视化的编程环境；

(2)   Win32 API是32位Windows操作系以C/C++形式提供的一组应用程序接口；

(3)   MFC是对Win32 API的封装，简化了开发过程。

 

111.怎样消除多重继承中的二义性?

答： 1．成员限定符  2．虚基类

 

112什么叫静态关联，什么叫动态关联

答：在多态中，如果程序在编译阶段就能确定实际执行动作，则称静态关联，

如果等到程序运行才能确定叫动态关联。

 

113多态的两个必要条件

答：1.一个基类的指针或引用指向一个派生类对象  2.虚函数

 

114.什么叫智能指针?

答：当一个类中，存在一个指向另一个类对象的指针时，对指针运算符进行重载，那么当前类对象可以通过指针像调用自身成员一样调用另一个类的成员。

 

115.什么时候需要用虚析构函数?

答：当基类指针指向用new运算符生成的派生类对象时，delete基类指针时，派生类部分没有释放掉而造成释放不彻底现象，需要虚析构函数。 补充：虚函数就是让派生类调用基类的虚函数。

 

116. MFC中，大部分类是从哪个类继承而来?

答：CObject

 

117.什么是平衡二叉树?

答：左右子树都是平衡二叉树，而且左右子树的深度差值的约对值不大于1。

 

118.语句for( ；1 ；)有什么问题?它是什么意思?

答：无限循环，和while(1)相同。

 

119．派生新类的过程要经历三个步骤

答：1.吸收基类成员    2.改造基类成员    3.添加新成员

 

121. TCP/IP 建立连接的过程

答：在TCP/IP协议中，TCP协议提供可靠的连接服务，采用三次握手建立一个连接。

第一次握手：建立连接时，客户端发送连接请求到服务器，并进入SYN_SEND状态，等待服务器确认；

第二次握手：服务器收到客户端连接请求，向客户端发送允许连接应答，此时服务器进入SYN_RECV状态；

第三次握手：客户端收到服务器的允许连接应答，向服务器发送确认，客户端和服务器进入通信状态，完成三次握手

 

122. memset ,memcpy 的区别

答：memset用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为’\0′。

memcpy用来做内存拷贝，你可以拿它拷贝任何数据类型的对象，可以指定拷贝的数据长度；

 

123. 在C++ 程序中调用被 C 编译器编译后的函数，为什么要加 extern “C”?

答：C++语言支持函数重载，C 语言不支持函数重载。函数被C++编译后在库中的名字

与C 语言的不同。假设某个函数的原型为： void foo(int x, int y);该函数被C 编译器编译后在库中的名字为_foo ， 而C++编译器则会产生像_foo_int_int 之类的名字。C++提供了C 连接交换指定符号extern“C”来解决名字匹配问题。

 

124怎样定义一个纯虚函数?含有纯虚函数的类称为什么?

答：在虚函数的后面加=0，含有虚函数的类称为抽象类。

 

125．已知strcpy函数的原型是：

char * strcpy(char * strDest,const char * strSrc);不调用库函数，实现strcpy函数。其中，strSrc是原字符串，strDest是目标字符串 。

答案：

char *strcpy(char *strDest, const char *strSrc)

{

if ( strDest == NULL || strSrc == NULL)

return NULL ;

if ( strDest == strSrc)

return strDest ;

char *tempptr = strDest ;  //指针tempptr指向strDest的地址；

while( (*strDest++ = *strSrc++) != ‘\\0’)  //注意：别忘了转义符；

;

return tempptr ;  //返回指针向的地址；

}

 

126．已知类String 的原型为：

class String

{

public:

String(const char *str = NULL);    // 普通构造函数

String(const String &other);  // 拷贝构造函数

~ String(void);                         // 析构函数

String & operate =(const String &other); // 赋值函数

private:

char *m_data;                                // 用于保存字符串

};

请编写String 的上述4 个函数。

答案：

// 普通构造函数

String::String(const char *str)

{

if ( str == NULL )         //strlen在参数为NULL时会抛异常才会有这步判断

{

m_data = new char[1] ;

m_data[0] = ” ;

}

else

{

m_data = new char[strlen(str) + 1];

strcpy(m_data,str);

}

}

//拷贝构造函数

String::String(const String &other)

{

m_data = new char[strlen(other.m_data) + 1];

strcpy(m_data,other.m_data);

}

//赋值函数(重载运算符)

String & String::operator =(const String &other)

{

if ( this == &other)

return *this ;

delete []m_data;

m_data = new char[strlen(other.m_data) + 1];

strcpy(m_data,other.m_data);

return *this ;

}

//析构函数

String::~ String(void)

{

delete []m_data ;

}

 

127．类成员函数的重载、覆盖和隐藏的区别

答案：

成员函数被重载的特征：

（1）相同的范围（在同一个类中）；

（2）函数名字相同；

（3）参数不同；

（4）virtual 关键字可有可无。

覆盖是指派生类函数覆盖基类函数，特征是：

（1）不同的范围（分别位于派生类与基类）；

（2）函数名字相同；

（3）参数相同；

（4）基类函数必须有virtual 关键字。

“隐藏”是指派生类的函数屏蔽了与其同名的基类函数，规则如下：

（1）如果派生类的函数与基类的函数同名，但是参数不同。此时，不论有无virtual关键字，基类的函数将被隐藏（注意别与重载混淆）。

（2）如果派生类的函数与基类的函数同名，并且参数也相同，但是基类函数没有virtual 关键字。此时，基类的函数被隐藏（注意别与覆盖混淆）

 

128．如何打印出当前源文件的文件名以及源文件的当前行号?

答案：

cout << __FILE__ ;

cout<<__LINE__ ;

__FILE__和__LINE__是系统预定义宏，这种宏并不是在某个文件中定义的，而是由编译器定义的。

 

129．文件中有一组整数，要求排序后输出到另一个文件中

答案：

void Order(vector &data)  //冒泡排序

{

int count = data.size() ;

int tag = false ;

for ( int i = 0 ; i < count ; i++)

{

for ( int j = 0 ; j < count – i – 1 ; j++)

{

if ( data[j] > data[j+1])

{

tag = true ;

int temp = data[j] ;

data[j] = data[j+1] ;

data[j+1] = temp ;

}

}

if ( !tag )

break ;

}

}

 

void main( void )

{

vectordata;

ifstream in(“c:\\data.txt”);

if ( !in)

{

cout<<”file error!”;

exit(1);

}

int temp;

while (!in.eof())

{

in>>temp;

data.push_back(temp);

}

in.close();

Order(data);

ofstream out(“c:\\result.txt”);

if ( !out)

{

cout<<”file error!”;

exit(1);

}

for ( i = 0 ; i < data.size() ; i++)

out<<data[i]<<” “;

out.close();

}

 

130．一个链表的结点结构  

struct Node

{

int data ;

Node *next ;

};

typedef struct Node Node ;

已知链表的头结点head,写一个函数把这个链表逆序 ( Intel)

答案：

Node * ReverseList(Node *head) //链表逆序

{

if ( head == NULL || head->next == NULL )

return head;

Node *p1 = head ;

Node *p2 = p1->next ;

Node *p3 = p2->next ;

p1->next = NULL ;

while ( p3 != NULL )

{

p2->next = p1 ;

p1 = p2 ;

p2 = p3 ;

p3 = p3->next ;

}

p2->next = p1 ;

head = p2 ;

return head ;

}

 

131. 一个链表的结点结构

struct Node

{

int data ;

Node *next ;

};

typedef struct Node Node ;

已知两个链表head1 和head2 各自有序，请把它们合并成一个链表依然有序。

答案：

Node * Merge(Node *head1 , Node *head2)

{

if ( head1 == NULL)

return head2 ;

if ( head2 == NULL)

return head1 ;

Node *head = NULL ;

Node *p1 = NULL;

Node *p2 = NULL;

if ( head1->data data )

{

head = head1 ;

p1 = head1->next;

p2 = head2 ;

}

else

{

head = head2 ;

p2 = head2->next ;

p1 = head1 ;

}

Node *pcurrent = head ;

while ( p1 != NULL && p2 != NULL)

{

if ( p1->data data )

{

pcurrent->next = p1 ;

pcurrent = p1 ;

p1 = p1->next ;

}

else

{

pcurrent->next = p2 ;

pcurrent = p2 ;

p2 = p2->next ;

}

}

if ( p1 != NULL )

pcurrent->next = p1 ;

if ( p2 != NULL )

pcurrent->next = p2 ;

return head ;

}

 

132.已知两个链表head1 和head2 各自有序，请把它们合并成一个链表依然有序，这次要求用递归方法进行。 ( Autodesk)

答案：

Node * MergeRecursive(Node *head1 , Node *head2)

{

if ( head1 == NULL )

return head2 ;

if ( head2 == NULL)

return head1 ;

Node *head = NULL ;

if ( head1->data data )

{

head = head1 ;

head->next = MergeRecursive(head1->next,head2);

}

else

{

head = head2 ;

head->next = MergeRecursive(head1,head2->next);

}

return head ;

}

 

133．分析一下这段程序的输出 (Autodesk)

class B

{

public:

B()

{

cout<<”default constructor”<<endl;

}

~B()

{

cout<<”destructed”<<endl;

}

B(int i):data(i)

{

cout<<”constructed by parameter” << data <<endl;

}

private:

int data;

};

B Play( B b)

{

return b ;

}

int main(int argc, char* argv[])

{

B temp = Play(5);

return 0;

}

 

133 将“引用”作为函数参数有哪些特点?

 

（1）传递引用给函数与传递指针的效果是一样的。这时，被调函数的形参就成为原来主调函数中的实参变量或对象的一个别名来使用，所以在被调函数中对形参变量的操作就是对其相应的目标对象（在主调函数中）的操作。

 

（2）使用引用传递函数的参数，在内存中并没有产生实参的副本，它是直接对实参操作；而使用一般变量传递函数的参数，当发生函数调用时，需要给形参分配存储单元，形参变量是实参变量的副本；如果传递的是对象，还将调用拷贝构造函数。因此，当参数传递的数据较大时，用引用比用一般变量传递参数的效率和所占空间都好。

 

（3）使用指针作为函数的参数虽然也能达到与使用引用的效果，但是，在被调函数中同样要给形参分配存储单元，且需要重复使用”*指针变量名”的形式进行运算，这很容易产生错误且程序的阅读性较差；另一方面，在主调函数的调用点处，必须用变量的地址作为实参。而引用更容易使用，更清晰。

 

134. 什么时候需要“引用”?

流操作符（>）和赋值操作符（=）的返回值、拷贝构造函数的参数、赋值操作符的参数、其它情况都推荐使用引用。

 

135.面向对象的三个基本特征，并简单叙述之?

 

1. 封装：将客观事物抽象成类，每个类对自身的数据和方法实行protection(private, protected,public)

 

2. 继承：广义的继承有三种实现形式：实现继承（指使用基类的属性和方法而无需额外编码的能力）、可视继承（子窗体使用父窗体的外观和实现代码）、接口继承（仅使用属性和方法，实现滞后到子类实现）。前两种（类继承）和后一种（对象组合=>接口继承以及纯虚函数）构成了功能复用的两种方式。

 

3. 多态：是将父对象设置成为和一个或更多的他的子对象相等的技术，赋值之后，父对象就可以根据当前赋值给它的子对象的特性以不同的方式运作。简单的说，就是一句话：允许将子类类型的指针赋值给父类类型的指针。

 

 

136.求下面函数的返回值（微软）

 

int func(x)

{

int countx = 0;

while(x)

{

countx ++;

x = x&(x-1);

}

return countx;

}

 

假定x = 9999。 答案：8

思路：将x转化为2进制，看含有的1的个数。

 

137、写出下列代码的输出内容

＃i nclude

int inc(int a)

{

return(++a);

}

int multi(int*a,int*b,int*c)

{

return(*c=*a**b);

}

typedef int(FUNC1)(int in);

typedef int(FUNC2) (int*,int*,int*);

void show(FUNC2 fun,int arg1, int*arg2)

{

INCp=&inc;

int temp =p(arg1);

fun(&temp,&arg1, arg2);

printf(“%d\n”,*arg2);

}

main()

{

int a;

show(multi,10,&a);

return 0;

}

答：110

138。编写一个 C 函数，该函数在一个字符串中找到可能的最长的子字符串，且该字符串是由同一字符组成的。

char * search(char *cpSource, char ch)

{

char *cpTemp=NULL, *cpDest=NULL;

int iTemp, iCount=0;

while(*cpSource)

{

if(*cpSource == ch)

{

iTemp = 0;

cpTemp = cpSource;

while(*cpSource == ch)

++iTemp, ++cpSource;

if(iTemp > iCount)

iCount = iTemp, cpDest = cpTemp;

if(!*cpSource)

break;

}

++cpSource;

}

return cpDest;

}

 

139。请编写一个 C 函数，该函数在给定的内存区域搜索给定的字符，并返回该字符所在位置索引值。

int search(char *cpSource, int n, char ch)

{

int i;

for(i=0; i<n && *(cpSource+i) != ch; ++i);

return i;

}

140.一个单向链表，不知道头节点,一个指针指向其中的一个节点，问如何删除这个指针指向的节点?

将这个指针指向的next节点值copy到本节点，将next指向next->next,并随后删除原next指向的节点。

141、用指针的方法，将字符串“ABCD1234efgh”前后对调显示

＃i nclude 

＃i nclude 

＃i nclude 

int main()

{

char str[] = “ABCD1234efgh”;

int length = strlen(str);

char * p1 = str;

char * p2 = str + length – 1;

while(p1 < p2)

{

char c = *p1;

*p1 = *p2;

*p2 = c;

++p1;

–p2;

}

printf(“str now is %s\n”,str);

system(“pause”);

return 0;

}

 

142、有一分数序列：1/2,1/4,1/6,1/8……，用函数调用的方法，求此数列前20项的和

＃i nclude 

double getValue()

{

double result = 0;

int i = 2;

while(i < 42)

{

result += 1.0 / i;//一定要使用1.0做除数，不能用1，否则结果将自动转化成整数，即0.000000

i += 2;

}

return result;

}

int main()

{

printf(“result is %f\n”, getValue());

system(“pause”);

return 0;

}

143、有一个数组a[1000]存放0–1000;要求每隔二个数删掉一个数，到末尾时循环至开头继续进行，求最后一个被删掉的数的原始下标位置。

以7个数为例：

{0,1,2,3,4,5,6,7} 0–>1–>2（删除）–>3–>4–>5(删除)–>6–>7–>0（删除），如此循环直到最后一个数被删除。

方法1：数组

＃include 

using namespace std;

#define null 1000

int main()

{

int arr[1000];

for (int i=0;i<1000;++i)

arr[i]=i;

int j=0;

int count=0;

while(count<999)

{

while(arr[j%1000]==null)

j=(++j)%1000;

j=(++j)%1000;

while(arr[j%1000]==null)

j=(++j)%1000;

j=(++j)%1000;

while(arr[j%1000]==null)

j=(++j)%1000;

arr[j]=null;

++count;

}

while(arr[j]==null)

j=(++j)%1000;

cout<<j<<endl;

return 0;

}

方法2：链表

＃i nclude

using namespace std;

#define null 0

struct node

{

int data;

node* next;

};

int main()

{

node* head=new node;

head->data=0;

head->next=null;

node* p=head;

for(int i=1;i<1000;i++)

{

node* tmp=new node;

tmp->data=i;

tmp->next=null;

head->next=tmp;

head=head->next;

}

head->next=p;

while(p!=p->next)

{

p->next->next=p->next->next->next;

p=p->next->next;

}

coutdata;

return 0;

}

 

方法3：通用算法

＃i nclude 

#define MAXLINE 1000   //元素个数

/*

MAXLINE   元素个数

a[]       元素数组

R[]       指针场

suffix    下标

index     返回最后的下标序号

values    返回最后的下标对应的值

start     从第几个开始

K         间隔

*/

int find_n(int a[],int R[],int K,int& index,int& values,int s=0) {

int suffix;

int front_node,current_node;

suffix=0;

if(s==0) {

current_node=0;

front_node=MAXLINE-1;

}

else {

current_node=s;

front_node=s-1;

}

while(R[front_node]!=front_node) {

printf(“%d\n”,a[current_node]);

R[front_node]=R[current_node];

if(K==1) {

current_node=R[front_node];

continue;

}

for(int i=0;i<K;i++){

front_node=R[front_node];

}

current_node=R[front_node];

}

index=front_node;

values=a[front_node];

return 0;

}

int main(void) {

int a[MAXLINE],R[MAXLINE],suffix,index,values,start,i,K;

suffix=index=values=start=0;

K=2;

for(i=0;i<MAXLINE;i++) {

a[i]=i;

R[i]=i+1;

}

R[i-1]=0;

find_n(a,R,K,index,values,2);

printf(“the value is %d,%d\n”,index,values);

return 0;

}

144、指出下列程序有什么错误：

void test2()

{

char string[10], str1[10];

int i;

for(i=0; i<10; i++)

{

str1[i] = ‘a’;

}

strcpy( string, str1 );

}

解答：对试题2，如果面试者指出字符数组str1不能在数组内结束可以给3分；如果面试者指出strcpy(string, str1)调用使得从str1内存起复制到string内存起所复制的字节数具有不确定性可以给7分，在此基础上指出库函数strcpy工作方式的给10分；

str1不能在数组内结束:因为str1的存储为：{a,a,a,a,a,a,a,a,a,a},没有’\0′(字符串结束符)，所以不能结束

strcpy( char *s1,char *s2)他的工作原理是，扫描s2指向的内存，逐个字符付到s1所指向的内存，直到碰到’\0′,因为str1结尾没有’\0′，所以具有不确定性，不知道他后面还会付什么东东。

正确应如下

void test2()

{

char string[10], str1[10];

int i;

for(i=0; i<9; i++)

{

str1[i] = ‘a’+i; //把abcdefghi赋值给字符数组

}

str[i]=’\0′;//加上结束符

strcpy( string, str1 );

}

145、实现strcmp

int StrCmp(const char *str1, const char *str2)

{

assert(str1 && srt2);

while(*str1 && *str1++ = = *str2++);

return *str1-*str2;

}

146.符串A和B,输出A和B中的最大公共子串。

比如A=”aocdfe” B=”pmcdfa” 则输出”cdf”

*/

//Author: azhen

＃i nclude

＃i nclude

＃i nclude

char *commanstring(char shortstring[], char longstring[])

{

int i, j;

char *substring=malloc(256);

if(strstr(longstring, shortstring)!=NULL)              //如果……，那么返回shortstring

return shortstring;

for(i=strlen(shortstring)-1;i>0; i–)                 //否则，开始循环计算

{

for(j=0; j<=strlen(shortstring)-i; j++){

memcpy(substring, &shortstring[j], i);

substring[i]=’\0′;

if(strstr(longstring, substring)!=NULL)

return substring;

}

}

return NULL;

}

 

main()

{

char *str1=malloc(256);

char *str2=malloc(256);

char *comman=NULL;

gets(str1);

gets(str2);

if(strlen(str1)>strlen(str2))                         //将短的字符串放前面

comman=commanstring(str2, str1);

else

comman=commanstring(str1, str2);

printf(“the longest comman string is: %s\n”, comman);

}

 

147、写一个函数比较两个字符串str1和str2的大小，若相等返回0，若str1大于

str2返回1，若str1小于str2返回－1

int strcmp ( const char * src,const char * dst)

{

int ret = 0 ;

while( ! (ret = *(unsigned char *)src – *(unsigned char *)dst) && *dst)

{

++src;

++dst;

}

if ( ret < 0 )

ret = -1 ;

else if ( ret > 0 )

ret = 1 ;

return( ret );

}

148、判断一个字符串是不是回文

int IsReverseStr(char *aStr)

{

int i,j;

int found=1;

if(aStr==NULL)

return -1;

j=strlen(aStr);

for(i=0;i<j/2;i++)

if(*(aStr+i)!=*(aStr+j-i-1))

{

found=0;

break;

}

return found;

 

149 #include main()

{

int c[3][3]={1,2,3,4,5,6,7,8,9};

for(int i=0;i<3;i++)

for(int j=0;j<3;j++)

printf(“%ld\n”,&c[j]);

printf(“————————-\n”);

printf(“%ld\n”,(c+1));

printf(“%ld\n”,(*c+1));

printf(“%ld\n”,&c[0][0]);

printf(“%ld\n”,**c);

printf(“%ld\n”,*c[0]);

if(int(c)==int(*c)) printf(“equl”);

}

为什么c，*c的值相等，（c+1），（*c+1）的值不等 c，*c，**c，代表什么意思?

参考答案：

c是第一个元素的地址，*c是第一行元素的首地址，其实第一行元素的地址就是第一个元素的地址，这容易理解。**c是提领第一个元素。

为什么c，*c的值相等?

int c因为直接用c表示数组c[0][0] printf(“%ld\n”,*c[0]);语句已将指针移到数组头。 int(*c)表示c0的值为1，所以相等。 数组c的存放空间示意如下：（机器中是行优先存放的） c[0][0] c[0][1] c[0][2] c[1][0] c[1][1] c[1][2] c[2][0] c[2][1] c[2][2] c是一个二维数组名，实际上它是一个指针常量，不能进行自加、自减运算，即：c++、c–、++c、–c 都是不允许的；

c： 数组名；是一个二维指针，它的值就是数组的首地址，也即第一行元素的首地址（等于 *c），也 等于第一行第一个元素的地址（ & c[0][0]）；可以说成是二维数组的行指针。 *c： 第一行元素的首地址；是一个一维指针，可以说成是二维数组的列指针。 **c：二维数组中的第一个元素的值；即：c[0][0] 所以： c 和 *c的值是相等的，但他们两者不能相互赋值，（类型不同）； （c + 1） ：c是行指针，（c + 1）是在c的基础上加上二维数组一行的地址长度，即从&c[0][0] 变到了&c[1][0]； （*c + 1）：*c是列指针，（*c + 1）是在*c的基础上加上二数组一个元素的所占的长度，即从 &c[0][0]变到了&c[0][1] 从而（c + 1）和（*c + 1）的值就不相等了

 

150、定义 int **a[3][4], 则变量占有的内存空间为：__32___ 参考答案：

int **p; /*16位下sizeof(p)=2, 32位下sizeof(p)=4*/

总共 3*4*sizeof(p)



题目（一）：我们可以用static修饰一个类的成员函数，也可以用const修饰类的成员函数（写在函数的最后表示不能修改成员变量，不是指写在前面表示返回值为常量）。请问：能不能同时用static和const修饰类的成员函数？

分析：答案是不可以。C++编译器在实现const的成员函数的时候为了确保该函数不能修改类的实例的状态，会在函数中添加一个隐式的参数const this*。但当一个成员为static的时候，该函数是没有this指针的。也就是说此时const的用法和static是冲突的。

我们也可以这样理解：两者的语意是矛盾的。static的作用是表示该函数只作用在类型的静态变量上，与类的实例没有关系；而const的作用是确保函数不能修改类的实例的状态，与类型的静态变量没有关系,因此不能同时用。

题目（二）：运行下面的代码，输出是什么？

class A
{
};
 
class B
{
public:
        B() {}
        ~B() {}
};
 
class C
{
public:
        C() {}
        virtual ~C() {}
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        printf("%d, %d, %d\n", sizeof(A), sizeof(B), sizeof(C));
        return 0;
}
分析：答案是1, 1, 4。class A是一个空类型，它的实例不包含任何信息，本来求sizeof应该是0。但当我们声明该类型的实例的时候，它必须在内存中占有一定的空间，否则无法使用这些实例。至于占用多少内存，由编译器决定。Visual Studio 2008中每个空类型的实例占用一个byte的空间。

class B在class A的基础上添加了构造函数和析构函数。由于构造函数和析构函数的调用与类型的实例无关（调用它们只需要知道函数地址即可），在它的实例中不需要增加任何信息。所以sizeof(B)和sizeof(A)一样，在Visual Studio 2008中都是1。

class C在class B的基础上把析构函数标注为虚拟函数。C++的编译器一旦发现一个类型中有虚拟函数，就会为该类型生成虚函数表，并在该类型的每一个实例中添加一个指向虚函数表的指针。在32位的机器上，一个指针占4个字节的空间，因此sizeof(C)是4。

题目（三）：运行下面中的代码，得到的结果是什么？

class A
{
private:
        int m_value;
 
public:
        A(int value)
        {
                m_value = value;
        }
        void Print1()
        {
                printf("hello world");
        }
        void Print2()
        {
                printf("%d", m_value);
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A* pA = NULL;
        pA->Print1();
        pA->Print2();
 
        return 0;
}

分析：答案是Print1调用正常，打印出hello world，但运行至Print2时，程序崩溃。调用Print1时，并不需要pA的地址，因为Print1的函数地址是固定的。编译器会给Print1传入一个this指针，该指针为NULL，但在Print1中该this指针并没有用到。只要程序运行时没有访问不该访问的内存就不会出错，因此运行正常。在运行print2时，需要this指针才能得到m_value的值。由于此时this指针为NULL，因此程序崩溃了。

题目（四）：运行下面中的代码，得到的结果是什么？

class A
{
private:
        int m_value;
 
public:
        A(int value)
        {
                m_value = value;
        }
        void Print1()
        {
                printf("hello world");
        }
        virtual void Print2()
        {
                printf("hello world");
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A* pA = NULL;
        pA->Print1();
        pA->Print2();
 
        return 0;
}

分析：答案是Print1调用正常，打印出hello world，但运行至Print2时，程序崩溃。Print1的调用情况和上面的题目一样，不在赘述。由于Print2是虚函数。C++调用虚函数的时候，要根据实例（即this指针指向的实例）中虚函数表指针得到虚函数表，再从虚函数表中找到函数的地址。由于这一步需要访问实例的地址（即this指针），而此时this指针为空指针，因此导致内存访问出错。

题目（五）：静态成员函数能不能同时也是虚函数？

分析：答案是不能。调用静态成员函数不要实例。但调用虚函数需要从一个实例中指向虚函数表的指针以得到函数的地址，因此调用虚函数需要一个实例。两者相互矛盾。

题目（六）：运行下列C++代码，输出什么？

struct Point3D
{
        int x;
        int y;
        int z;
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        Point3D* pPoint = NULL;
        int offset = (int)(&(pPoint)->z);
 
        printf("%d", offset);
        return 0;
}

答案：输出8。由于在pPoint->z的前面加上了取地址符号，运行到此时的时候，会在pPoint的指针地址上加z在类型Point3D中的偏移量8。由于pPoint的地址是0，因此最终offset的值是8。

&(pPoint->z)的语意是求pPoint中变量z的地址（pPoint的地址0加z的偏移量8），并不需要访问pPoint指向的内存。只要不访问非法的内存，程序就不会出错。

题目（七）：运行下列C++代码，输出什么？

class A
{
public:
        A()
        {
                Print();
        }
        virtual void Print()
        {
                printf("A is constructed.\n");
        }
};
 
class B: public A
{
public:
        B()
        {
                Print();
        }
 
        virtual void Print()
        {
                printf("B is constructed.\n");
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A* pA = new B();
        delete pA;
 
        return 0;
}

答案：先后打印出两行:A is constructed. B is constructed. 调用B的构造函数时，先会调用B的基类及A的构造函数。然后在A的构造函数里调用Print。由于此时实例的类型B的部分还没有构造好，本质上它只是A的一个实例，他的虚函数表指针指向的是类型A的虚函数表。因此此时调用的Print是A::Print，而不是B::Print。接着调用类型B的构造函数，并调用Print。此时已经开始构造B，因此此时调用的Print是B::Print。

同样是调用虚拟函数Print，我们发现在类型A的构造函数中，调用的是A::Print，在B的构造函数中，调用的是B::Print。因此虚函数在构造函数中，已经失去了虚函数的动态绑定特性。

题目（八）：运行下列C#代码，输出是什么？

namespace ChangesOnString
{
    class Program
    {
        static void Main(string[] args)
        {
            String str = "hello";
            str.ToUpper();
            str.Insert(0, " WORLD");
 
            Console.WriteLine(str);
        }
    }
}
答案：输出是hello。由于在.NET中，String有一个非常特殊的性质：String的实例的状态不能被改变。如果String的成员函数会修改实例的状态，将会返回一个新的String实例。改动只会出现在返回值中，而不会修改原来的实例。所以本题中输出仍然是原来的字符串值hello。

如果试图改变String的内容，改变之后的值可以通过返回值拿到。用StringBuilder是更好的选择，特别是要连续多次修改的时候。如果用String连续多次修改，每一次修改都会产生一个临时对象，开销太大。

题目（九）：在C++和C#中，struct和class有什么不同？

答案：在C++中，如果没有标明函数或者变量是的访问权限级别，在struct中，是public的；而在class中，是private的。

                在C#中，如果没有标明函数或者变量的访问权限级别，struct和class中都是private的。struct和class的区别是：struct定义值类型，其实例在栈上分配内存；class定义引用类型，其实例在堆上分配内存。

题目（十）：运行下图中的C#代码，输出是什么？

namespace StaticConstructor
{
    class A
    {
        public A(string text)
        {
            Console.WriteLine(text);
        }
    }
 
    class B
    {
        static A a1 = new A("a1");
        A a2 = new A("a2");
 
        static B()
        {
            a1 = new A("a3");
        }
 
        public B()
        {
            a2 = new A("a4");
        }
    }
 
    class Program
    {
        static void Main(string[] args)
        {
            B b = new B();
        }
    }
}

答案：打印出四行，分别是a1、a3、a2、a4。

在调用类型B的代码之前先执行B的静态构造函数。静态函数先初始化类型的静态变量，再执行静态函数内的语句。因此先打印a1再打印a3。接下来执行B b = new B()，即调用B的普通构造函数。构造函数先初始化成员变量，在执行函数体内的语句，因此先后打印出a2、a4。

题目（11）：运行下图中的C#代码，输出是什么？

namespace StringValueOrReference
{
    class Program
    {
        internal static void ValueOrReference(Type type)
        {
            String result = "The type " + type.Name;
 
            if (type.IsValueType)
                Console.WriteLine(result + " is a value type.");
            else
                Console.WriteLine(result + " is a reference type.");
        }
 
        internal static void ModifyString(String text)
        {
            text = "world";
        }
 
        static void Main(string[] args)
        {
            String text = "hello";
 
            ValueOrReference(text.GetType());
            ModifyString(text);
 
            Console.WriteLine(text);
        }
    }
}
答案：输出两行。第一行是The type String is reference type. 第二行是hello。类型String的定义是public sealed class String {...}，既然是class，那么String就是引用类型。

在方法ModifyString里，对text赋值一个新的字符串，此时改变的不是原来text的内容，而是把text指向一个新的字符串"world"。由于参数text没有加ref或者out，出了方法之后，text还是指向原来的字符串，因此输出仍然是"hello".

题目（12）：运行下图中的C++代码，输出是什么？

#include <iostream>
 
class A
{
private:
        int n1;
        int n2;
public:
        A(): n2(0), n1(n2 + 2)
        {
        }
 
        void Print()
        {
                std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl;
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A a;
        a.Print();
 
        return 0;
}

答案：输出n1是一个随机的数字，n2为0。在C++中，成员变量的初始化顺序与变量在类型中的申明顺序相同，而与它们在构造函数的初始化列表中的顺序无关。因此在这道题中，会首先初始化n1，而初始n1的参数n2还没有初始化，是一个随机值，因此n1就是一个随机值。初始化n2时，根据参数0对其初始化，故n2=0。

题目（13）：编译运行下图中的C++代码，结果是什么？（A）编译错误；（B）编译成功，运行时程序崩溃；（C）编译运行正常，输出10。请选择正确答案并分析原因。

#include <iostream>
 
class A
{
private:
        int value;
 
public:
        A(int n)
        {
                value = n;
        }
 
        A(A other)
        {
                value = other.value;
        }
 
        void Print()
        {
                std::cout << value << std::endl;
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A a = 10;
        A b = a;
        b.Print();
 
        return 0;
}

答案：编译错误。在复制构造函数中传入的参数是A的一个实例。由于是传值，把形参拷贝到实参会调用复制构造函数。因此如果允许复制构造函数传值，那么会形成永无休止的递归并造成栈溢出。因此C++的标准不允许复制构造函数传值参数，而必须是传引用或者常量引用。在Visual Studio和GCC中，都将编译出错。

题目（14）：运行下图中的C++代码，输出是什么？

int SizeOf(char pString[])
{
        return sizeof(pString);
}
 
int _tmain(int argc, _TCHAR* argv[])
{
        char* pString1 = "google";
        int size1 = sizeof(pString1);
        int size2 = sizeof(*pString1);
 
        char pString2[100] = "google";
        int size3 = sizeof(pString2);
        int size4 = SizeOf(pString2);
 
        printf("%d, %d, %d, %d", size1, size2, size3, size4);
 
        return 0;
}

答案：4, 1, 100, 4。pString1是一个指针。在32位机器上，任意指针都占4个字节的空间。*pString1是字符串pString1的第一个字符。一个字符占一个字节。pString2是一个数组，sizeof(pString2)是求数组的大小。这个数组包含100个字符，因此大小是100个字节。而在函数SizeOf中，虽然传入的参数是一个字符数组，当数组作为函数的参数进行传递时，数组就自动退化为同类型的指针。因此size4也是一个指针的大小，为4.

题目（15）：运行下图中代码，输出的结果是什么？这段代码有什么问题？

#include <iostream>
 
class A
{
public:
        A()
        {
                std::cout << "A is created." << std::endl;
        }
 
        ~A()
        {
                std::cout << "A is deleted." << std::endl;
        }
};
 
class B : public A
{
public:
        B()
        {
                std::cout << "B is created." << std::endl;
        }
 
        ~B()
        {
                std::cout << "B is deleted." << std::endl;
        }
};
 
int _tmain(int argc, _TCHAR* argv[])
{
        A* pA = new B();
        delete pA;
 
        return 0;
}

答案：输出三行，分别是：A is created. B is created. A is deleted。用new创建B时，回调用B的构造函数。在调用B的构造函数的时候，会先调用A的构造函数。因此先输出A is created. B is created.

接下来运行delete语句时，会调用析构函数。由于pA被声明成类型A的指针，同时基类A的析构函数没有标上virtual，因此只有A的析构函数被调用到，而不会调用B的析构函数。

由于pA实际上是指向一个B的实例的指针，但在析构的时候只调用了基类A的析构函数，却没有调用B的析构函数。这就是一个问题。如果在类型B中创建了一些资源，比如文件句柄、内存等，在这种情况下都得不到释放，从而导致资源泄漏。

问题（16）：运行如下的C++代码，输出是什么？

class A
{
public:
    virtual void Fun(int number = 10)
    {
        std::cout << "A::Fun with number " << number;
    }
};
 
class B: public A
{
public:
    virtual void Fun(int number = 20)
    {
        std::cout << "B::Fun with number " << number;
    }
};
 
int main()
{
    B b;
    A &a = b;
    a.Fun();
}

答案：输出B::Fun with number 10。由于a是一个指向B实例的引用，因此在运行的时候会调用B::Fun。但缺省参数是在编译期决定的。在编译的时候，编译器只知道a是一个类型a的引用，具体指向什么类型在编译期是不能确定的，因此会按照A::Fun的声明把缺省参数number设为10。

            这一题的关键在于理解确定缺省参数的值是在编译的时候，但确定引用、指针的虚函数调用哪个类型的函数是在运行的时候。

问题（17）：运行如下的C代码，输出是什么？

char* GetString1()
{
    char p[] = "Hello World";
    return p;
}
 
char* GetString2()
{
    char *p = "Hello World";
    return p;
}
 
 
int _tmain(int argc, _TCHAR* argv[])
{
    printf("GetString1 returns: %s. \n", GetString1());
    printf("GetString2 returns: %s. \n", GetString2());
 
    return 0;
}

答案：输出两行，第一行GetString1 returns: 后面跟的是一串随机的内容，而第二行GetString2 returns: Hello World. 两个函数的区别在于GetString1中是一个数组，而GetString2中是一个指针。

当运行到GetString1时，p是一个数组，会开辟一块内存，并拷贝"Hello World"初始化该数组。接着返回数组的首地址并退出该函数。由于p是GetString1内的一个局部变量，当运行到这个函数外面的时候，这个数组的内存会被释放掉。因此在_tmain函数里再去访问这个数组的内容时，结果是随机的。

当运行到GetString2时，p是一个指针，它指向的是字符串常量区的一个常量字符串。该常量字符串是一个全局的，并不会因为退出函数GetString2而被释放掉。因此在_tmain中仍然根据GetString2返回的地址得到字符串"Hello World"。

问题（18）：运行下图中C#代码，输出的结果是什么？

namespace StaticVariableInAppDomain
{
    [Serializable]
    internal class A : MarshalByRefObject
    {
        public static int Number;
 
        public void SetNumber(int value)
        {
            Number = value;
        }
    }
 
    [Serializable]
    internal class B
    {
        public static int Number;
 
        public void SetNumber(int value)
        {
            Number = value;
        }
    }
 
    class Program
    {
        static void Main(string[] args)
        {
            String assamblyName = Assembly.GetEntryAssembly().FullName;
            AppDomain domain = AppDomain.CreateDomain("NewDomain");
 
            A.Number = 10;
            String nameOfA = typeof(A).FullName;
            A a = domain.CreateInstanceAndUnwrap(assamblyName, nameOfA) as A;
            a.SetNumber(20);
            Console.WriteLine("Number in class A is {0}", A.Number);
 
            B.Number = 10;
            String nameOfB = typeof(B).FullName;
            B b = domain.CreateInstanceAndUnwrap(assamblyName, nameOfB) as B;
            b.SetNumber(20);
            Console.WriteLine("Number in class B is {0}", B.Number);
        }
    }
}
答案：输出两行，第一行是Number in class A is 10，而第二行是Number in class B is 20。上述C#代码先创建一个命名为NewDomain的应用程序域，并在该域中利用反射机制创建类型A的一个实例和类型B的一个实例。我们注意到类型A是继承自MarshalByRefObject，而B不是。虽然这两个类型的结构一样，但由于基类不同而导致在跨越应用程序域的边界时表现出的行为将大不相同。

      由于A继承MarshalByRefObject，那么a实际上只是在缺省的域中的一个代理，它指向位于NewDomain域中的A的一个实例。当a.SetNumber时，是在NewDomain域中调用该方法，它将修改NewDomain域中静态变量A.Number的值并设为20。由于静态变量在每个应用程序域中都有一份独立的拷贝，修改NewDomain域中的静态变量A.Number对缺省域中的静态变量A.NewDomain没有任何影响。由于Console.WriteLine是在缺省的应用程序域中输出A.Number，因此输出仍然是10。

    B只从Object继承而来的类型，它的实例穿越应用程序域的边界时，将会完整地拷贝实例。在上述代码中，我们尽管试图在NewDomani域中生成B的实例，但会把实例b拷贝到缺省的域。此时，调用b.SetNumber也是在缺省的域上进行，它将修改缺省的域上的A.Number并设为20。因此这一次输出的是20。

问题（19）：运行下图中C代码，输出的结果是什么？

int _tmain(int argc, _TCHAR* argv[])
{
    char str1[] = "hello world";
    char str2[] = "hello world";
 
    char* str3 = "hello world";
    char* str4 = "hello world";
 
    if(str1 == str2)
        printf("str1 and str2 are same.\n");
    else
        printf("str1 and str2 are not same.\n");
 
    if(str3 == str4)
        printf("str3 and str4 are same.\n");
    else
        printf("str3 and str4 are not same.\n");
 
    return 0;
}

答案：输出两行。第一行是str1 and str2 are not same，第二行是str3 and str4 are same。

str1和str2是两个字符串数组。我们会为它们分配两个长度为12个字节的空间，并把"hello world"的内容分别拷贝到数组中去。这是两个初始地址不同的数组，因此比较str1和str2的值，会不相同。str3和str4是两个指针，我们无需为它们分配内存以存储字符串的内容，而只需要把它们指向"hello world“在内存中的地址就可以了。由于"hello world”是常量字符串，它在内存中只有一个拷贝，因此str3和str4指向的是同一个地址。因此比较str3和str4的值，会是相同的。

问题（20）：运行下图中C#代码，输出的结果是什么？并请比较这两个类型各有什么特点，有哪些区别。

namespace Singleton
{
    public sealed class Singleton1
    {
        private Singleton1()
        {
            Console.WriteLine("Singleton1 constructed");
        }
        public static void Print()
        {
            Console.WriteLine("Singleton1 Print");
        }
        private static Singleton1 instance = new Singleton1();
        public static Singleton1 Instance
        {
            get
            {
                return instance;
            }
        }
    }
 
    public sealed class Singleton2
    {
        Singleton2()
        {
            Console.WriteLine("Singleton2 constructed");
        }
        public static void Print()
        {
            Console.WriteLine("Singleton2 Print");
        }
        public static Singleton2 Instance
        {
            get
            {
                return Nested.instance;
            }
        }
        class Nested
        {
            static Nested() { }
 
            internal static readonly Singleton2 instance = new Singleton2();
        }
    }
 
    class Program
    {
        static void Main(string[] args)
        {
            Singleton1.Print();
            Singleton2.Print();
        }
    }
}
答案: 输出三行：第一行“Singleton1 constructed”，第二行“Singleton1 Print”，第三行“Singleton2 Print”。

当我们调用Singleton1.Print时，.NET运行时会自动调用Singleton1的静态构造函数，并初始化它的静态变量。此时会创建一个Singleton1的实例，因此会调用它的构造函数。Singleton2的实例是在Nested的静态构造函数里初始化的。只有当类型Nested被使用时，才回触发.NET运行时调用它的静态构造函数。我们注意到我们只在Sington2.Instance里面用到了Nested。而在我们的代码中，只调用了Singleton2.Print。因此不会创建Singleton2的实例，也不会调用它的构造函数。

这两个类型其实都是单例模式（Singleton）的实现。第二个实现Singleton2只在真的需要时，才会创建实例，而第一个实现Singleton1则不然。第二个实现在空间效率上更好。

问题（21）：C#是一门托管语言，那么是不是说明只要用C#，就能保证不会出现内存泄露和其他资源泄漏？如果不是，在哪些情况下可能会出现泄漏？

答案：C#不能保证没有资源泄漏。比如如下几种情况可能会造成资源泄漏：（1） 调用Native code，比如用P/Invoke或者调用COM；（2） 读写文件时的，没有及时close stream, 或者ADO.NET连数据库时，没有及时关闭连接，也算资源泄漏？（3）注册事件后没有remove，导致publisher和subscriber的强依 赖，垃圾回收可能会被推迟；（4）.NET还定义了一些方法直接申请非托管内存，比如Marshal.AllocHGlobal和Marshal.AllocCoTaskMem。通过这种方式得到的内存，如果没有及时释放，也会造成内存泄露。

问题（22）：下面的两段C#有哪些不同？

static void CatchException1()
{
    try
    {
        Function();
    }
    catch
    {
        throw;
    }
}
 
static void CatchException2()
{
    try
    {
        Function();
    }
    catch (Exception e)
    {
        throw e;
    }
}

答案：两个函数的catch都是重新抛出截获的exception，但抛出的exception的call stack是不一样的。对于第一种方法，exception的call stack是从最开始的抛出地点开始的。对于第二种方法，exception的call stack是从CatchException2开始的，最初抛出的地方相关的信息被隐藏了。

问题（23）：运行下图中的C++代码，打印出的结果是什么？

bool Fun1(char* str)
{
    printf("%s\n", str);
    return false;
}
 
bool Fun2(char* str)
{
    printf("%s\n", str);
    return true;
}
 
int _tmain(int argc, _TCHAR* argv[])
{
    bool res1, res2;
    res1 = (Fun1("a") && Fun2("b")) || (Fun1("c") || Fun2("d"));
    res2 = (Fun1("a") && Fun2("b")) && (Fun1("c") || Fun2("d"));
 
    return res1 || res2;
}

答案：打印出4行，分别是a、c、d、a。

在C/C++中，与、或运算是从左到右的顺序执行的。在计算rest1时，先计算Fun1(“a”) && Func2(“b”)。首先Func1(“a”)打印出内容为a的一行。由于Fun1(“a”)返回的是false, 无论Func2(“b”)的返回值是true还是false，Fun1(“a”) && Func2(“b”)的结果都是false。由于Func2(“b”)的结果无关重要，因此Func2(“b”)会略去而不做计算。接下来计算Fun1(“c”) || Func2(“d”)，分别打印出内容c和d的两行。

                在计算rest2时，首先Func1(“a”)打印出内容为a的一行。由于Func1(“a”)返回false，和前面一样的道理，Func2(“b”)会略去不做计算。由于Fun1(“a”) && Func2(“b”)的结果是false，不管Fun1(“c”) && Func2(“d”)的结果是什么，整个表达式得到的结果都是false，因此Fun1(“c”) && Func2(“d”)都将被忽略。

问题（24）：运行下面的C#代码，打印出来的结果是什么？

struct Person
{
    public string Name;
 
    public override string ToString()
    {
        return Name;
    }
}
 
class Program
{
    static void Main(string[] args)
    {
        ArrayList array = new ArrayList();
        Person jim = new Person() {Name = "Jim"};
        array.Add(jim);
 
        Person first = (Person)array[0];
        first.Name = "Peter";
        Console.WriteLine(array[0].ToString());
    }
}

答案：Person的定义是一个struct，因此是一个值类型。在运行到语句Person first = (Person)array[0]的时候，first是array[0]的一个拷贝，first和array[0]不是一个实例。因此修改first对array[0]没有影响。

问题（25）：运行下面的C++代码，打印的结果是什么？

class Base
{
public:
    void print() { doPrint();}
 
private:
    virtual void doPrint() {cout << "Base::doPrint" << endl;}
};
 
class Derived : public Base
{
private:
    virtual void doPrint() {cout << "Derived::doPrint" << endl;}
};
 
int _tmain(int argc, _TCHAR* argv[])
{
    Base b;
    b.print();
 
    Derived d;
    d.print();
 
       return 0;
}

答案：输出两行，分别是Base::doPrint和Derived::doPrint。在print中调用doPrint时，doPrint()的写法和this->doPrint()是等价的，因此将根据实际的类型调用对应的doPrint。所以结果是分别调用的是Base::doPrint和Derived::doPrint2。如果感兴趣，可以查看一下汇编代码，就能看出来调用doPrint是从虚函数表中得到函数地址的。
