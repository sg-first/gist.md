void quicksort(int left,int right,vector<int>&sortvec)
{
    if(left>right) //加减过度了说明全部处理完毕，递归结束
       return;

    int i,j;
    int base=sortvec[left]; //基准数，这里选最左的
    //左右游标从两端出发
    i=left;
    j=right;

    while(i!=j) //左右游标相遇时本次排序结束
    {
        //基准数选最左，所以右边游标先移动。二者分别在左右找到一个失序的数
        while(sortvec[j]>=base && i<j)
            j--;
        while(sortvec[i]<=base && i<j)
            i++;

        if(i<j) //交换左右游标找到的两个失序的数
        {
            int t=sortvec[i];
            sortvec[i]=sortvec[j];
            sortvec[j]=t;
        }
    }

    //此时基准数应该在本次left和right的中心，所以交换基准数和中心
    sortvec[left]=sortvec[i];
    sortvec[i]=base;

    quicksort(left,i-1,sortvec); //处理左边
    quicksort(i+1,right,sortvec); //处理右边
}