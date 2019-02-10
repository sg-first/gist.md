# 生成一个奇数生成器
def odd_iter():
    n = 1
    while True:
        n = n + 2
        yield n

# 过滤掉n的倍数的数
def not_divisible(n):
    return lambda x: x % n > 0

# 获取当前序列的第一个元素，然后删除后面序列该元素倍数的数，然后构造新序列
def primes():
    yield 2
    it = odd_iter()
    while True:
        n = next(it)
        yield n
        it = filter(not_divisible(n), it)

# 获取 start 到 stop 之间的素数
def rangePrime(start, stop=-1):
    for n in primes():
        if stop!=-1:
            if n > start and n < stop:
                yield n
            elif n > stop:
                break
        else:
            if n>start:
                yield n