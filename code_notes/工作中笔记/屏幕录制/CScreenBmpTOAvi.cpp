/*************************************************
 类ScreenBmpTOAvi的实现文件
 以/***开始的注释是我个人觉得比较难理解的地方
************x-ke***********************************/

#include "StdAfx.h"
#include "CScreenBmpTOAvi.h"

CScreenBmpTOAvi::CScreenBmpTOAvi()
{
	InitMemVariable();

}
void CScreenBmpTOAvi::InitMemVariable()                         //把要手动释放的变量全设置为NULL
{
	m_hScrDC=NULL;
	m_hMemDC=NULL;
	m_lpData=NULL;
	m_pStreamCompressed=NULL;
	m_pAviStream=NULL;
	m_pAviFile=NULL;
	m_hBitmap=NULL;
}
void CScreenBmpTOAvi::ReleaseResourse()                         //释放资源，要是全面不把变量设为NULL的话，可能会出现异常
{
	if(m_hScrDC)
	{
		DeleteDC(m_hScrDC);
		m_hScrDC=NULL;
	}
	if(m_hMemDC)
	{
		DeleteDC(m_hMemDC);
		m_hMemDC=NULL;
	}
	if (m_lpData) 
	{
		GlobalFree(m_lpData);
		m_lpData=NULL;
	}
	if (m_pStreamCompressed)
	{
		AVIStreamRelease(m_pStreamCompressed);
		m_pStreamCompressed=NULL;
	}
	if (m_pAviStream) 
	{
		AVIStreamRelease(m_pAviStream);
		m_pAviStream=NULL;
	}
	if (m_pAviFile) 
	{
        AVIFileRelease(m_pAviFile);
		m_pAviFile=NULL;
	}
	if (m_hBitmap) {
		DeleteObject(m_hBitmap);
		m_hBitmap=NULL;
	}

}

CScreenBmpTOAvi::CScreenBmpTOAvi(LPCSTR outPutPath,CRect region)                //构造函数，目的是获得捕获窗体
{
	  InitMemVariable();
	  AVIFileInit();
      m_nFreamCount=0;
	  m_strPath=outPutPath;
	  int  nX,nY,nX2,nY2;
	  nX=region.left;
	  nY=region.top;
      nX2=region.right;
	  nY2=region.bottom;
	  m_hScrDC  =     CreateDC("DISPLAY",   NULL,   NULL,   NULL);//这个函数得到的DC为桌面DC 
	  int xScrn   =   GetDeviceCaps(m_hScrDC,   HORZRES);//获得整个屏幕X，Y    
      int yScrn   =   GetDeviceCaps(m_hScrDC,   VERTRES);

	  //////////////////这一大堆if是为了让我们的区域不能跑到屏幕外去老///////////////
	  if   (nX<0)   
                nX=0;    
      if   (nY<0)   
                nY=0;   
      if   (nX2>xScrn)   
                nX2= xScrn;   
      if   (nY2>yScrn)   
                nY2=yScrn; 
	  m_nTop=nY;
	  m_nLeft=nX;
      this->m_nWidth=nX2-nX;   
      this->m_nHeight=nY2-nY;

}
CScreenBmpTOAvi::CScreenBmpTOAvi(LPCSTR outPutPath,CPoint lefttop,CPoint rightbottom) //构造函数，目的是获得捕获窗体
{
	  InitMemVariable();
	  AVIFileInit();
      m_nFreamCount=0;
	  m_strPath=outPutPath;
	  int  nX,nY,nX2,nY2;
	  nX=lefttop.x;
	  nY=lefttop.y;
      nX2=rightbottom.x;
	  nY2=rightbottom.y;
	  m_hScrDC  =     CreateDC("DISPLAY",   NULL,   NULL,   NULL);//这个函数得到的DC为桌面DC 
	  int xScrn   =   GetDeviceCaps(m_hScrDC,   HORZRES);//获得整个屏幕X，Y    
      int yScrn   =   GetDeviceCaps(m_hScrDC,   VERTRES);

	  //////////////////这一大堆if是为了让我们的区域不能跑到屏幕外去老///////////////
	  if   (nX<0)   
                nX=0;    
      if   (nY<0)   
                nY=0;   
      if   (nX2>xScrn)   
                nX2= xScrn;   
      if   (nY2>yScrn)   
                nY2=yScrn; 
	  m_nTop=nY;
	  m_nLeft=nX;
      this->m_nWidth=nX2-nX;   
      this->m_nHeight=nY2-nY;
	
}
CScreenBmpTOAvi::CScreenBmpTOAvi(LPCSTR outPutPath,int left,int top,int right,int bottom )//构造函数，目的是获得捕获窗体
{
	  InitMemVariable();
	  AVIFileInit();
      m_nFreamCount=0;
	  m_strPath=outPutPath;
	  int  nX,nY,nX2,nY2;
	  nX=left;
	  nY=top;
      nX2=right;
	  nY2=bottom;
	  m_hScrDC  =     CreateDC("DISPLAY",   NULL,   NULL,   NULL); //这个函数得到的DC为桌面DC
	  int xScrn   =   GetDeviceCaps(m_hScrDC,   HORZRES);//获得整个屏幕X，Y    
      int yScrn   =   GetDeviceCaps(m_hScrDC,   VERTRES);

	  //////////////////这一大堆if是为了让我们的区域不能跑到屏幕外去老///////////////
	  if   (nX<0)   
                nX=0;    
      if   (nY<0)   
                nY=0;   
      if   (nX2>xScrn)   
                nX2= xScrn;   
      if   (nY2>yScrn)   
                nY2=yScrn; 
	  m_nTop=nY;
	  m_nLeft=nX;
      m_nWidth=nX2-nX;   
      m_nHeight=nY2-nY;

	    /* COMPVARS m_CV;
           memset(&m_CV,0,sizeof(COMPVARS));
           //设置COMPVARS结构
           m_CV.dwFlags=AVICOMPRESSF_KEYFRAMES|ICMF_COMPVARS_VALID;
           //COMPVARS结构大小
           m_CV.cbSize=sizeof(m_CV);
           //使用divx编码器
           m_CV.fccHandler=mmioFOURCC('d','i','v','x');
           //编码器类型
           m_CV.fccType=ICTYPE_VIDEO;
           //打开编码器,hic为编码器句柄
           //m_CV.hic=ICOpen(ICTYPE_VIDEO,mmioFOURCC('d','i','v','x'),ICMODE_COMPRESS);
           //这里调用总返回0.
           ICCompressorChoose((AfxGetApp()->m_pMainWnd)->GetSafeHwnd(),
			   ICMF_CHOOSE_DATARATE|ICMF_CHOOSE_KEYFRAME,
			   NULL,
			   NULL,
			   &m_CV,
			   "请选择压缩方式.."); 

				ZeroMemory(&m_AviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	            m_AviCompressOptions.fccType=m_CV.fccType;                       //表示视频                      
	            m_AviCompressOptions.fccHandler=m_CV.fccHandler;                               //压缩编码
	            m_AviCompressOptions.dwFlags=m_CV.dwFlags;//|AVICOMPRESSF_DATARATE;
	            m_AviCompressOptions.dwKeyFrameEvery=1;                                //每多少帧设置1个关键帧，现在全是关键帧
	            m_AviCompressOptions.dwBytesPerSecond=1000/8;                          
	            m_AviCompressOptions.dwQuality=7500; */                                  //压缩质量


}
CScreenBmpTOAvi::CScreenBmpTOAvi(LPCSTR outPutPath)//构造函数，目的是获得捕获窗体
{
	  InitMemVariable();
	  AVIFileInit();
	  m_nFreamCount=0;
	  m_strPath=outPutPath;
	  m_hScrDC  =     CreateDC("DISPLAY",   NULL,   NULL,   NULL); //这个函数得到的DC为桌面DC
	  int xScrn   =   GetDeviceCaps(m_hScrDC,   HORZRES);//获得整个屏幕X，Y    
      int yScrn   =   GetDeviceCaps(m_hScrDC,   VERTRES);
      m_nHeight=yScrn;
	  m_nWidth=xScrn;
	  m_nTop=0;
	  m_nLeft=0;

}
CScreenBmpTOAvi::CScreenBmpTOAvi(LPCSTR outPutPath, CWnd* pWnd)//构造函数，目的是获得捕获窗体
{
	InitMemVariable();
	AVIFileInit();
	m_nFreamCount=0;
	m_strPath=outPutPath;
	//m_hScrDC  =     CreateDC("DISPLAY",   NULL,   NULL,   NULL); //这个函数得到的DC为桌面DC
	m_hScrDC = pWnd->GetDC()->GetSafeHdc();
	int xScrn   =   GetDeviceCaps(m_hScrDC,   HORZRES);//获得整个屏幕X，Y    
	int yScrn   =   GetDeviceCaps(m_hScrDC,   VERTRES);
	m_nHeight=yScrn;
	m_nWidth=xScrn;
	m_nTop=0;
	m_nLeft=0;

}
LRESULT CScreenBmpTOAvi::CreateBmpInfo(BOOL bCursor)      //从获得的窗体中获得位图信息
{
	   

          m_hMemDC  =   CreateCompatibleDC(m_hScrDC);//为屏幕设备描述表创建兼容的内存设备描述表
	      
          //   创建一个与屏幕设备描述表兼容的位图   
          m_hBitmap   =   CreateCompatibleBitmap(m_hScrDC,   m_nWidth,   m_nHeight);   
          //   把新位图选到内存设备描述表中   
  
          HBITMAP hOldBitmap;
	      hOldBitmap = (HBITMAP)SelectObject(m_hMemDC,   m_hBitmap);
		  //   把屏幕设备描述表拷贝到内存设备描述表中
          BitBlt(m_hMemDC, 0, 0,m_nWidth, m_nHeight,m_hScrDC,m_nLeft,m_nTop,SRCCOPY);
		  //***这个函数的意思是把m_hScrDC上的以m_nLeft，m_nTop为左上角宽为m_nWidth，高为m_nHeight
		  //***拷贝到内存DC中以0，0为左上角，宽和高为m_nWidth，m_nHeight的区域中去


		  /////////////////////////得到鼠标信息/////////////////////////////////////
		   if (TRUE==bCursor) {
			 HCURSOR hCursor=GetCursor();//获取当前光标及其位置
             POINT ptCursor;
             GetCursorPos(&ptCursor);
             ICONINFO IconInfo; //获取光标的图标数据 

             if (GetIconInfo(hCursor, &IconInfo))
			 {
                 ptCursor.x -= ((int) IconInfo.xHotspot);
                 ptCursor.y -= ((int) IconInfo.yHotspot);
                 if (IconInfo.hbmMask != NULL)
                  DeleteObject(IconInfo.hbmMask);
                 if (IconInfo.hbmColor != NULL)
                  DeleteObject(IconInfo.hbmColor);
			 }
          /////////////////在兼容设备描述表上画出该光标////////////////////////
            DrawIconEx(
                 m_hMemDC, // 设备上下文句柄 
                 ptCursor.x-m_nLeft,
				 ptCursor.y-m_nTop,//位置
                 hCursor, // 要画的光标句柄
                 0,
				 0, // width of the icon 
                 0, // index of frame in animated cursor 
                 NULL, // handle to background brush 
                 DI_NORMAL|DI_MASK|DI_IMAGE // icon-drawing flags 
                 );
		///////////////////////////////////////////////////////////////////////
			DWORD dw=GetLastError();

	     } 
	  //得到屏幕位图的句柄   
       m_hBitmap   = (HBITMAP)  SelectObject(m_hMemDC,   hOldBitmap);       
	  ///////////////////////
       DeleteObject(hOldBitmap);
       hOldBitmap=NULL;
//////////下面这段代码的目的是得到一个BITMAP结构，不用CBitmap而直接用HBITMAP好像也可以////
	   CBitmap   bm;
	   bm.Attach(m_hBitmap);
       BITMAP   btm;   
       bm.GetBitmap(&btm);
/////////////////////////////////////
	   DWORD   size=btm.bmWidthBytes*btm.bmHeight;     //bmp 数据大小，它等于宽度所占的字节数乘以位图高
       if (0==m_nFreamCount) {
       
	   m_lpData=(LPSTR)GlobalAlloc(GPTR,size);         //***分配内存空间，类似于C的ALLOC
	   }
//////////////////////////填充位图结构/////
	   if (0==m_nFreamCount) {
	   ZeroMemory(&m_bmpInfo,sizeof(BITMAPINFO));                    //*****ZeroMemory这个函数比较常见，就是把一段长度为sizeof(..)内存区初始化，有点象给指针变量分配空间 
	                                                                 //BITMAPINFO这个结构由BITMAPINFOHEADER ，RGBQUAD  2个部分组成具体信息请看MSDN 
	   m_bmpInfo.bmiHeader.biPlanes		= 1;                         //必须为1               
	   m_bmpInfo.bmiHeader.biWidth		= btm.bmWidth;               //位图宽         
	   m_bmpInfo.bmiHeader.biHeight		= btm.bmHeight;              //位图高
	   m_bmpInfo.bmiHeader.biCompression	= BI_RGB;                //位图压缩格式 BI_RGB为不压缩，也不知道能不能把压缩了的位图写进去，那位大哥没事可以式式
	   m_bmpInfo.bmiHeader.biBitCount	= btm.bmBitsPixel;           //设定每个象素占多少个位
	   m_bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);  //这个值表示位图头占了好多字节
	   m_bmpInfo.bmiHeader.biSizeImage= m_bmpInfo.bmiHeader.biWidth
		                              *m_bmpInfo.bmiHeader.biHeight
									  *m_bmpInfo.bmiHeader.biBitCount
									  /8;                             //图像大小，但也不晓得为啥子要整成这个值
	   }
	   /////////////////////////////////////////////////////////////////////////
	   /****GetDIBits这个函数的作用是用特定能格式，把位图里面的一段数据取到一个缓冲区中 
///////////////////////////////////////////////////////////////////////*/
	   /*GetDIBits(m_hMemDC,                                   //位图所在的DC
		         m_hBitmap,                                  //位图句柄
				 0,                                          //
				 m_bmpInfo.bmiHeader.biHeight,
				 NULL,                      //？？？？？？？？？？为什么用NULL我还不清楚
				 (BITMAPINFO*)&m_bmpInfo,
				 DIB_RGB_COLORS
				 );*/                        
	   GetDIBits(m_hMemDC,
		         m_hBitmap,
				 0,
				 m_bmpInfo.bmiHeader.biHeight,
				 (LPVOID)m_lpData,
				 &m_bmpInfo,
				 DIB_RGB_COLORS);//把位图数据取道m_lpdata中
	   DeleteDC(m_hMemDC);
	   m_hMemDC=NULL;
	   return S_OK;
}

////////////////////////////总算可以准备写AVI了/////////////////////////////////////

LRESULT  CScreenBmpTOAvi::ScreenToAviStream(
			COMPVARS &cmopVers,
			DWORD dwKeyFrameEvery,
			DWORD dwFrameRate,						   
			DWORD dwCodec
			)
{
	LRESULT hr=S_OK;
	if(0==m_nFreamCount)//如果第一帧的话那么还得先打开文件流和写AVI文件头。
             {
			    __try
				{
                AVIFileOpen(&m_pAviFile,m_strPath,OF_WRITE | OF_CREATE,NULL);  //打开路径名为lpszFileNamAVI文件，以创建和写的方式
			////////////////////设定文件流信息，AVISTREAMINFO结构///////////
                memset(&m_StreamInfo, 0, sizeof(AVISTREAMINFO));                     //这个函数和ZEROMEMORY效果一个样
                m_StreamInfo.fccType                = streamtypeVIDEO;               //表示为视频
                m_StreamInfo.fccHandler             =dwCodec;                       //压缩控制
                m_StreamInfo.dwScale                = 1;
                m_StreamInfo.dwRate                 = dwFrameRate;                 //rate除以scale就是帧频
				m_StreamInfo.dwQuality=-1;                            
                m_StreamInfo.dwSuggestedBufferSize = m_bmpInfo.bmiHeader.biSizeImage ;//输入缓冲区，一般设为图片大小
                SetRect(&m_StreamInfo.rcFrame,
					    0,
						0,
						m_bmpInfo.bmiHeader.biWidth,
						m_bmpInfo.bmiHeader.biHeight
						);                                                            //设置AVI中图片存在的窗体大小

                _tcscpy(m_StreamInfo.szName,_T("Video Stream"));                      //名字，随便取
		  ////////////////////////创建流/////////////////////////////

                hr = AVIFileCreateStream(m_pAviFile,&m_pAviStream,&m_StreamInfo);   
				
		  ///////////////压缩相关//////////////////////

    
//		///////////////////////////////////////////////////////
				ZeroMemory(&m_AviCompressOptions,sizeof(AVICOMPRESSOPTIONS));
	            m_AviCompressOptions.fccType=streamtypeVIDEO;                          //表示视频                      
	            m_AviCompressOptions.fccHandler=dwCodec;                               //压缩编码
	            m_AviCompressOptions.dwFlags=AVICOMPRESSF_KEYFRAMES|AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	            m_AviCompressOptions.dwKeyFrameEvery=dwKeyFrameEvery;                                //每多少帧设置1个关键帧，现在全是关键帧
	            m_AviCompressOptions.dwBytesPerSecond=1000/8;                          
	            m_AviCompressOptions.dwQuality=cmopVers.lQ;                                   //压缩质量
//           ///////////////////////////////////////////     
	             if(AVIERR_OK!=(AVIMakeCompressedStream(&m_pStreamCompressed,
					                                     m_pAviStream,
														 &m_AviCompressOptions,
														 NULL)))                        //创建压缩AVI流                       
				 {
				   hr= E_FAIL;
		
				 }
		      AVIStreamSetFormat(
					        m_pStreamCompressed,
					        m_nFreamCount,
					        (LPVOID)&m_bmpInfo,
					        sizeof(BITMAPINFOHEADER));
				}
			__except(EXCEPTION_EXECUTE_HANDLER )
			{
				hr= E_FAIL;

			}
	}

///////////////写入文件流////////////////////////
            __try
			{
			
                AVIStreamWrite(m_pStreamCompressed,                // stream pointer
			                   m_nFreamCount,                    // time of this frame
                               1,                                  // number to write
                               (LPVOID) m_lpData,
                               m_bmpInfo.bmiHeader.biSizeImage ,   // size of this frame
                               0,                                  // flags....
                               NULL,
							   NULL);
			}
			__except(EXCEPTION_EXECUTE_HANDLER )
			{
				hr= E_FAIL;
			}
			
	m_nFreamCount++;
	//GlobalFree(m_lpData);
	//m_lpData=NULL;
	return hr;
}
