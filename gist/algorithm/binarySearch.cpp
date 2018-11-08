int binarySearch(int arr[],int low,int high,int value)
{
	int mid=low+(high-low)/2;
    if(arr[mid]==value)
        return mid;
    if(arr[mid]>value)
        return binarySearch(arr,low,mid-1,value);
    if(arr[mid]<value)
        return binarySearch(arr,mid+1,high,value);
}
