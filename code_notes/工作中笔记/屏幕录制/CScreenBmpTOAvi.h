/*****************************************************
这个类完成的是获取屏幕一个区域BMP，然后把这个BMP写入到 
一个AVI文件中去
需要在工程里引入vfw32.lib，需要MFC

||||****注意*********************||||
1、ScreenToAviStream()这个函数调用使用一定要判断它是否已经成功，不然就有的受了！
   不成功就要直接停止录制！！
2、不要同时录制2个区域哦，小心死机！！！！
3、用完一定要记得ReleaseResourse()哦;不然你录得AVI不是打不开就啥子都没有！
||||*****************************||||
调用说明：
    CScreenBmpTOAvi bmp(300,300,600,600);
    //CScreenBmpTOAvi bmp1(CRect(200,400,500,700));
	//CScreenBmpTOAvi bmp2(CPoint(0,0),CPoint(500,500));
	//CScreenBmpTOAvi bmp3;
    for (int i=0;i<100;i++) {
		//bmp1.CreateBmpInfo(); 
		//bmp2.CreateBmpInfo();
		//bmp3.CreateBmpInfo();

		bmp.CreateBmpInfo();
		if(E_FAIL==bmp.ScreenToAviStream())
		{
			break;
		}

		//if(E_FAIL==bmp1.ScreenToAviStream(_T("outputrect.avi")))
	//	{
	//		break;
		//}
	//	if(E_FAIL==bmp2.ScreenToAviStream(_T("outputpoint.avi")))
		//{
		//	break;
		//}
		//if(E_FAIL==bmp3.ScreenToAviStream(_T("outputfullscreen.avi")))
		//{
		//	break;
		//}
    	Sleep(100);
    }
//	bmp1.ReleaseResourse();
//	bmp2.ReleaseResourse();
//	bmp3.ReleaseResourse();
	bmp.ReleaseResourse();
****************x-ke***07**4***27*********************/
#pragma comment(lib,"vfw32.lib") 

#include <vfw.h>
#ifndef __BMPTOAVI
#define __BMPTOAVI

class CScreenBmpTOAvi{
private:                                            //-------------------成员变量------------------------||
	int  m_nxScrn,m_nyScrn;                         //记录当前屏幕分辨率
	int  m_nLeft,m_nTop;                            //这2个参数确定录制区域的左上角位置         
	int  m_nWidth,m_nHeight;                        //区域的长和高，它们和前二个参数结合就可以找到我们要录制的区域
	HDC  m_hScrDC;                                  //屏幕DC，DC简单的说就是窗体上的一个区域
	HDC  m_hMemDC;                                  //与屏幕DC相对应的内存DC
	LPSTR   m_lpData;                               //一个数据指针，它将被用于保存图片里的数据
	LPCSTR  m_strPath;
	DWORD m_nFreamCount;                            //帧计数器
	HBITMAP m_hBitmap;                              //位图句柄，用于保存每次获得的位图信息
	BITMAPINFO m_bmpInfo;                           //位图信息结构
    PAVIFILE      m_pAviFile;                       //AVI文件指针，就相当于一个文件对象
    PAVISTREAM    m_pAviStream;                     //AVI流
	PAVISTREAM	  m_pStreamCompressed;              //AVI压缩流
	AVISTREAMINFO m_StreamInfo;                     //AVI视频流结构
    AVICOMPRESSOPTIONS	m_AviCompressOptions;       //压缩设置
                                                    //——————————————————————————||


	void InitMemVariable();                         //把要申请内存，还必须手动删除的成员全部设为NULL，以便在后面能正确地删除。

public:
	CScreenBmpTOAvi();
	CScreenBmpTOAvi(LPCSTR outPutPath);                                               //这个构造函数表示抓取的是整个屏幕区域    
	CScreenBmpTOAvi(LPCSTR outPutPath, CWnd* pWnd); 
	CScreenBmpTOAvi(LPCSTR outPutPath,CRect region);                                   //这个构造函数接受一个矩形区域对象
	CScreenBmpTOAvi(LPCSTR outPutPath,CPoint lefttop,CPoint rightbottom);	            //接受2个点
	CScreenBmpTOAvi(LPCSTR outPutPath,int left,int top,int right,int bottom);          //接受4个坐标	                                     
	LRESULT CreateBmpInfo(BOOL bCursor=FALSE);       //创建BMP信息和提取数据
	LRESULT ScreenToAviStream(                       //这个函数是把上一个函数获得的BMP信息和数据写到AVI视频中去，第一帧的话就创建AVI头
		    COMPVARS &cmopVers,
			DWORD dwKeyFrameEvery=3,
			DWORD dwFrameRate =2,					// 帧因素
			DWORD dwCodec= mmioFOURCC('M','S','V','C')             //压缩方式                    			
			);                                   
	void ReleaseResourse();                                          //释放资源

	void SetScrHDC(HDC hdcScr){ m_hScrDC = hdcScr;}
};
#endif