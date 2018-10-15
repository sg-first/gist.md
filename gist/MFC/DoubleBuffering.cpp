void OnDraw(CDC *pDC)
{
    //定义一个内存设备描述表对象
    CDC MemDC; 
    //定义一个位图对象
    CBitmap MemBitmap;
    //建立与屏幕设备描述表（前端缓冲区）兼容的内存设备描述表句柄（后备缓冲区）
    MemDC.CreateCompatibleDC(NULL);
    //这时还不能绘图，因为没有位图的设备描述表是不能绘图的
    //下面建立一个与屏幕设备描述表（或者内存设备描述表）兼容的位图
    MemBitmap.CreateCompatibleBitmap(pDC,nWidth,nHeight);
    //将位图选入到内存设备描述表
    //只有选入了位图的设备描述表才有地方绘图，画到指定的位图上
    CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);
 
    //先用背景色将位图清除干净，这里我用的是白色作为背景
    MemDC.FillSolidRect(0,0,nWidth,nHeight,RGB(255,255,255));
    //绘图
 
 
    //将后备缓冲区中的图形拷贝到前端缓冲区
    pDC->BitBlt(0,0,nWidth,nHeight,&MemDC,0,0,SRCCOPY);
    //绘图完成后的清理
    MemBitmap.DeleteObject();
    MemDC.DeleteDC();
 
    // 然后使用默认处理    CView::OnDraw();
}
 
// 添加WM_ERASEBKGND响应函数，并清除响应函数的生成代码在其中添加如下代码
 
BOOL OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //return CDialog::OnEraseBkgnd(pDC);
    return FALSE;
}