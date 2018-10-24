#include<vector>
using namespace std;

bool nextPermutation(vector<int> &nums)
{
    int size = nums.size();
    for (int i = size - 1; i > 0; --i)
    {
        //找到第二个划分与第一个划分的分界线
        if (nums[i] > nums[i - 1])
        {
            int val = nums[i - 1];
            int j = size - 1;
            //找到第一大于val值的位置
            for (; j >= i; --j)
            {
                if (nums[j] > val)
                    break;
            }
            //交换val值和第一个大于val的值
            swap(nums[i - 1], nums[j]);
            int l = i;
            int r = size - 1;
            //逆序处理
            while (l < r)
            {
                swap(nums[l], nums[r]);
                l++;
                r--;
            }
            return true;
        }
    }
    int start = 0;
    int last = size - 1;
    while (start < last)
    {
        swap(nums[start], nums[last]);
        start++;
        last--;
    }
    return false;
}