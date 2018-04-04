void createBigPagePDF_test(const char* fileName,bool flag = false,float userUnit = 1,float dpi=72.0f){
	// 画布毫米大小
    SkISize canvasMMSize = SkISize::Make(5.080 * 1000, 5.080 * 1000);
	// 画布转换到PDF中point大小
	SkISize pdfPointSize = SkISize::Make(mmToPoint(canvasMMSize.width()), mmToPoint(canvasMMSize.height()));
	SkRect r;			//绘制到pdf页面上的范围，单位是Point
	r.set(14400,14400,28340,28340);
	MaxminStruc mms;	//虚构网格座标范围
	mms.min_x = 100000;
	mms.min_y = 1000000;
	mms.max_x = 200000;
	mms.max_y = 2000000;

	SkMatrix tmat;		//用于转换网格座标和pdf绘制座标的矩阵
	tmat.reset();
	SkPoint srcPs[3],dstPs[3];
	srcPs[0] = SkPoint::Make(mms.min_x,mms.min_y);
	srcPs[1] = SkPoint::Make(mms.min_x,mms.max_y);
	srcPs[2] = SkPoint::Make(mms.max_x,mms.max_y);
	dstPs[0] = SkPoint::Make(r.left(),r.top());
	dstPs[1] = SkPoint::Make(r.left(),r.bottom());
	dstPs[2] = SkPoint::Make(r.right(),r.bottom());
	tmat.setPolyToPoly(srcPs,dstPs,3);

	SkMatrix mat;		//用于初始化画布的矩阵
	mat.reset();
	mat.postScale(1/userUnit,1/userUnit);

	SkPDFDevice* pdfDevice = new SkPDFDevice(pdfPointSize,pdfPointSize,mat);
	SkCanvas* fPdfCanvas = new SkCanvas(pdfDevice);
	SkPaint paint;
	paint.setAlpha(255);
	paint.setARGB(255,255,0,0);
	paint.setStrokeWidth(1);
	size_t xstep = (mms.max_x-mms.min_x)/10;
	size_t ystep = (mms.max_y-mms.min_y)/10;
	SkPoint p1,p2;		//绘制10*10网格线
	for(size_t i = mms.min_x;i < mms.max_x;i+=xstep){
		for(size_t j = mms.min_y; j < mms.max_y; j += ystep){
			tmat.mapXY(i,j,&p1);tmat.mapXY(i,j+ystep,&p2);
			fPdfCanvas->drawLine(p1.x(),p1.y(),p2.x(),p2.y(),paint);
			tmat.mapXY(i,j+ystep,&p1);tmat.mapXY(i+xstep,j+ystep,&p2);
			fPdfCanvas->drawLine(p1.x(),p1.y(),p2.x(),p2.y(),paint);
			tmat.mapXY(i+xstep,j+ystep,&p1);tmat.mapXY(i+xstep,j,&p2);
			fPdfCanvas->drawLine(p1.x(),p1.y(),p2.x(),p2.y(),paint);
			tmat.mapXY(i+xstep,j,&p1);tmat.mapXY(i,j,&p2);
			fPdfCanvas->drawLine(p1.x(),p1.y(),p2.x(),p2.y(),paint);
		}
	}
	SkString name (fileName);
	SkPDFDocument doc;
	doc.appendPage(pdfDevice);
	SkFILEWStream stream(name.c_str());		//保存数据
	if (stream.isValid()) {
		bool bEmit=doc.emitPDF(&stream,flag,userUnit);
	}
	delete fPdfCanvas;
	fPdfCanvas = NULL;
}