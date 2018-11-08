void bubbleSort(vector<int>&arr)
{
    for(int i=0;i<arr.size()-1;i++) //计数，最坏情况遍历size-1次
    {
        bool changed=false;
        for(int j=0;j<arr.size()-1-i;j++) //单次遍历，每次最右边都会多排好一个，所以那个不用看了
        {
            if(arr[j]>arr[j+1])
            {
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
                changed=true;
            }
        }

        if(!changed) //如果有一次遍历没有交换任何元素，说明已经全部排好
            break;
    }
}
