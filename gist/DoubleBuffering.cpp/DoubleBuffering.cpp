LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
 
    switch (message)
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // 分析菜单选择: 
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO:  在此添加任意绘图代码...
        MYDraw(hdc);                    // MYDraw处理
        ReleaseDC(hWnd, hdc)
        EndPaint(hWnd, &ps);
        break;
    case WM_ERASEBKGND:
        //return TRUE;
        break;
    case WM_DESTROY:
        Game.~CSnakeView();                    // 释放游戏对象
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
 
 
void MYDraw(HDC &pDC)
{
    // TODO:  在此处为本机数据添加绘制代码
    CRect DCRect;
    GetClientRect(hwnd,&DCRect);
    
    HDC compatibleDC = CreateCompatibleDC(NULL);        // 创建兼容DC
    
    HBITMAP bmp = CreateCompatibleBitmap(pDC, DCRect.Width(), DCRect.Height());
    HBITMAP oldbmp = (HBITMAP)SelectObject(compatibleDC, bmp);        // 加载兼容位图
 
    // 清空屏幕 设置背景颜色
    HBRUSH bgbr = CreateSolidBrush(BACKGROUNDCOLOR);
    FillRect(compatibleDC, DCRect, bgbr);
    DeleteObject(bgbr);
    
    // 画图
    
    // 从CompatibleDC拷贝到pDC
    BitBlt(pDC, 0, 0, DCRect.Width(), DCRect.Height(), compatibleDC, 0, 0, SRCCOPY);
    
    SelectObject(compatibleDC, oldbmp);
    DeleteObject(oldbmp);
    DeleteObject(bmp);
    DeleteDC(compatibleDC);    // 删除资源
 
    // ValidateRect(hwnd, &DCRect);
    // ReleaseDC(hwnd, pDC);
    // EndPaint 已完成这些功能
    return;
}