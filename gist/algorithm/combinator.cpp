long factorial(long number)
{
    if(number<=1)
        return 1;
    else
        return number*factorial(number-1);
}

int combinator(int n,int m)
{
    return factorial(n)/(factorial(m)*factorial(n-m));
}
