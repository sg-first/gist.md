function lcs(arr,brr)
{
    var i,j;
    var dp=[];
    var max=function(a,b){return (a>b?a:b);};
    var lena=arr.length;
    var lenb=brr.length;

    // core
    for(i=0;i<lena+1;i++)
    {
        dp[i]=[];
        for(j=0;j<lenb+1;j++)
        {
            if(i==0||j==0)
                dp[i][j]=0;
            else
            {
                if(arr[i]===brr[j])
                    dp[i][j]=dp[i-1][j-1]+1;
                else
                    dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
            }
        }
    }
    var lst=[];

    var traceback=function(i,j,lst)
    {
        while(i>0&&j>0)
        {
            if(arr[i-1]===brr[j-1])
            {
                lst.push(arr[i-1]);
                //if(lst.length===dp[lena][lenb]) //此时lst是找到的一个子序列
                i--;
                j--;
            }
            else
            {
                if(dp[i-1][j]>dp[i][j-1])
                    --i;
                else
                {
                    if(dp[i-1][j]<dp[i][j-1])
                        --j;
                    else
                    {
                        traceback(i-1,j,lst);
                        traceback(i,j-1,lst);
                        return;
                    }
                }
            }
        }
    };

    traceback(lena,lenb,lst);
    return dp[lena][lenb]; //公共子序列的最大长度
}