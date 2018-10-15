void savetoint(char *s, int *a,int weizhi)
{
    for (int j = 0; weizhi >= 0; j++, weizhi--)
    {
        a[j] = s[weizhi] - '0';
    }
}

void savetochar(int *a, char *s, int weizhi)
{
    int j = 0;
    for (; weizhi >= 0; j++, weizhi--)
    {
        s[j] = a[weizhi] + '0' ;
    }
    s[j] = '\0';
}

int plus(int *a,int *b,int size)
{
    for (int i=0; i <= size;i++)
    {
        b[i] += a[i];
        if (b[i] >= 10){
            b[i + 1] += b[i] / 10;
            b[i] %= 10;
        }
    }
    if (b[size + 1] != 0)
    {
        return size + 1;
    }
    else
    {
        return size;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    int a[200] = { 0 }, b[201]={0};
    char sa[201], sb[202];
    scanf_s("%s%s", sa, 200, sb, 200);
    int lena = strlen(sa) - 1, lenb = strlen(sb) - 1;
    savetoint(sa, a, lena);
    savetoint(sb, b, lenb);
    if (lena >= lenb)
    {
        savetochar(b, sb, plus(a, b, lena));
    }
    else
    {
        savetochar(a, sb, plus(b, a, lenb));
    }
    printf("%s\n", sb);
    system(_T("PAUSE"));
    return 0;
}