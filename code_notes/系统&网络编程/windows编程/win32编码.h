字符集
Windows使用两种字符集ANSI和UNICODE
ANSI通常使用单字节方式
UNICODE采用双字节方式，方便处理双字节字符，比如中文
如果编译一个程序为ANSI方式，则_T()不起作用，
如果编译一个程序为UNICODE方式，则编译器会把字符串已UNICODE方式保存
_T()和_L()的区别在于，_L()不管用哪种方式编译，一律以UNICODE方式保存

char 单字节字符类型，对应ANSI
wchar_t 宽字节字符类型，对应UNICODE
LPSTR: 本质是char*，32-bit指针指向一个字符串，每个字符占1字节
LPWSTR: 本质是wchar_t*，64-bit指针指向字符串，每个字符占2字节
LPCSTR: 32-bit指针指向一个常字符串，每个字符占1字节
LPCWSTR: 64-bit指针指向常字符串，该字符串不能通过指针修改，每个字符占2字节

TCHAR: 取决于Unicode是否定义
LPCTSTR: 32-bit指针指向一个常字符串,每字符可能占1字节或2字节，取决于Unicode是否定义
LPTSTR: 32-bit指针每字符可能占1字节或2字节，取决于Unicode是否定义
字符串都用 _T()这个宏转换，要使用 _T()需包含宏tchar.h

字符串转换
数字转CString
	float a = 1234.56；
	CString src；
	src.Format(_T("%f"),a);
CString转数字
  CString strLine("123.45 456.12");
  float a = 0.f;
	float b = 0.f;
	_stscanf_s(strLine,_T("%f %f"),&a,&b);
	
1. const char* 转换成char*
	被const修饰的指针不能赋给非const修饰的指针
	const char* ptr = "good";
	char* buf = new char[strlen(ptr)+1];
	strcpy(bug,ptr);
	或者
	const char* ptr = "good";
	char* buf = const_cast<char*>(ptr);
2. char* 转 const char*
	char* p = "good day";
	const char* q = p;
3. string 转 const char*
	string s = "abc";
	const char* c_s = s.c_str();
4. const char* 转 string
	const char* temp = "abc";
	string s = temp;
5. char* 转 string
	char* p = "good night";
	string str(p);
6. string 转 char*
	string str = "good night";
	const char* temp = str.c_str();
	char* dest = const_cast<char*>(temp);
	
字符串指针
1. char* str = "abcdefg"   初始化指针时所创建的字符串常量被定义为只读
2. char str[] = "abcdefg"  由字符串常量初始化的数组是可以修改的
3. char* p = "abcdefg";
   const char* q = "abcdefg"; //p和q两个指针是一样的，指向的数据都是只可读
   
   char a = 'b';
   char* p = &a;
   const char* q = &a;     //p和q两个指针不一样，p指针可以通过引用修改a中数据，而q指针不能
 
 
4.


Unicode 和ANSI相互装换
  int nwCh;
	char AnsiStr[] = "hello, world!";
	wchar_t wszBuf[20] = {0};
 
	//获得转换后产生多少Unicode字符，可以作为后面实际转换时传入容纳转换结果的Unicode字符数buffer大小
	nwCh = MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, NULL, 0);
	//转换并接收结果
	MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, wszBuf, nwCh);
	wprintf(L"nwCh = %d, %s\n", nwCh, wszBuf);
 
	int nCh;
	char AnsiBuf[20] = {0};
	//获得转换后产生多少ANSI字符，可以作为后面实际转换时传入容纳转换结果的ANSI字符数buffer大小
	nCh = WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, NULL, 0, NULL, NULL);
	//转换并接收结果
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, AnsiBuf, nCh, NULL, NULL);
  printf("nCh = %d, %s\n", nCh, AnsiBuf);
  