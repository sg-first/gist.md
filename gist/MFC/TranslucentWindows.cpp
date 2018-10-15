BOOL CAlphaDialogDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
 
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动执行此操作
    SetIcon(m_hIcon, TRUE);            // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标
 
    // TODO:  在此添加额外的初始化代码
 
 
    /* 32bpp透明窗体的实现
     *
     * 需要给窗体设置 WS_EX_LAYERED 属性
     * 可通过对话框Layered属性或SetWindowLong方法
     */
    HWND hwnd = GetSafeHwnd();        // 窗口句柄
    HDC hDC = ::GetDC(hwnd);        // 窗口DC
    RECT rc;
    ::GetWindowRect(hwnd, &rc);        // 窗口区域
 
    // 准备工作
    HDC hMemDc = ::CreateCompatibleDC(hDC);                // 创建兼容DC
    BITMAPINFO bmpinfo;                                    // 32bpp位图结构
    // BITMAPINFOHEADER 结构  https://msdn.microsoft.com/zh-cn/library/dd183376.aspx    
    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth = rc.right-rc.left;
    bmpinfo.bmiHeader.biHeight = rc.bottom-rc.top;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    // CreateDIBSection 函数 https://msdn.microsoft.com/zh-cn/data/dd183494
    void* pBits = NULL;
    HBITMAP hBitmap = CreateDIBSection(hDC,    &bmpinfo, DIB_RGB_COLORS, &pBits, NULL, 0);        // 创建32bpp位图
    HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDc, hBitmap);    // 加载兼容位图
 
    // 半透明绘图
    CImage img;
    img.Load(_T("C:\\Users\\Mooc\\Desktop\\egg.png"));    // 加载PNG图像
    TransparentPNG(&img);                                // PNG透明处理
    img.Draw(hMemDc, 0, 0);
 
    // 更新一个分层窗口的位置、大小、形状、内容和半透明。
    // BLENDFUNCTION 结构 https://msdn.microsoft.com/zh-cn/library/dd183393.aspx
    BLENDFUNCTION blf;
    blf.BlendOp = AC_SRC_OVER;
    blf.BlendFlags = 0;
    blf.SourceConstantAlpha = 255;
    blf.AlphaFormat = AC_SRC_ALPHA;
    POINT ptDest = { rc.left, rc.top };
    SIZE  sizeDest = { rc.right - rc.left, rc.bottom - rc.top };
    POINT ptSrc = { 0, 0 };
    // UpdateLayeredWindow 函数 https://msdn.microsoft.com/zh-cn/library/ms633556.aspx
    ::UpdateLayeredWindow(hwnd, hDC, &ptDest, &sizeDest, hMemDc, &ptSrc, 0, &blf, ULW_ALPHA);
 
    // 收尾操作
    ::SelectObject(hMemDc, hOldBitmap);
    ::DeleteObject(hBitmap);
    ::DeleteDC(hMemDc);
    ::ReleaseDC(hwnd, hDC);
    
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}