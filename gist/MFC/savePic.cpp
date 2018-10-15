void MySaveBmpTofile(HBITMAP hbmp, CString path)//hbmp：需保存的图像的句柄，path：保存路径
{
    //定义文件头结构
    BITMAPFILEHEADER fileHead;                                              
    int fileHeadLen = sizeof( BITMAPFILEHEADER );
 
    //定义图像信息结构
    BITMAPINFOHEADER bmpHead;
    int bmpHeadLen =sizeof( BITMAPINFOHEADER );
 
    BITMAP bmpObj;
    GetObject( hbmp, sizeof(BITMAP), &bmpObj );
 
    DWORD fileSizeInByte;//文件总的字节大小
 
    //获取系统颜色深度,即每个像素用多少位表还示
    DWORD PixelSizeInBit;               
    CDC srcDC;//系统屏幕设备描述表
 
    srcDC.CreateDC( "DISPLAY", NULL, NULL, NULL);
 
    PixelSizeInBit=srcDC.GetDeviceCaps( BITSPIXEL ) * srcDC.GetDeviceCaps( PLANES );
    fileSizeInByte = fileHeadLen + bmpHeadLen + bmpObj.bmWidth*bmpObj.bmHeight*PixelSizeInBit/8;
 
    //初始化文件头结构
    fileHead.bfOffBits = fileHeadLen + bmpHeadLen;
    fileHead.bfReserved1=0;
    fileHead.bfReserved2=0;
    fileHead.bfSize = fileSizeInByte;
    fileHead.bfType = 0x4D42;
 
    ///初始图像信息结构
    bmpHead.biBitCount = PixelSizeInBit;
    bmpHead.biCompression = BI_RGB;
    bmpHead.biPlanes = 1;
    bmpHead.biHeight = bmpObj.bmHeight;
    bmpHead.biWidth = bmpObj.bmWidth;
    bmpHead.biSize = bmpHeadLen;
 
    //为文件分配空间
    PBYTE pFile = new byte[ fileSizeInByte ];
    memset( pFile, 0, fileSizeInByte );
 
    //填充文件头部
    memcpy( pFile, (PBYTE)&fileHead, fileHeadLen);
 
    //填充文件信息头部结构
    memcpy( pFile+fileHeadLen, (PBYTE)&bmpHead, bmpHeadLen);
 
    //填充像素部分
    GetDIBits( srcDC.m_hDC, hbmp, 0, bmpObj.bmHeight, pFile+fileHeadLen+bmpHeadLen, (LPBITMAPINFO)(pFile+fileHeadLen), DIB_RGB_COLORS);
 
    //打开文件并写入数据
    HANDLE hFile;
    hFile=CreateFile( path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
 
    if(hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox( "创建文件失败" );
        return;
    }
 
    DWORD nByteTransfered;
    WriteFile( hFile, pFile, fileSizeInByte, &nByteTransfered, NULL);
    CloseHandle( hFile );
 
    //清理
    delete pFile;
    srcDC.DeleteDC();
}