import numpy as np
from matplotlib import pyplot as plt

def funImg2D(f, start=-10, stop=10, step=0.03, axisRange=(-10,10)):
    # 计算所有的点
    num=(stop-start)/step
    x = np.linspace(start,stop,num)
    y = f(x)

    fig=plt.figure(figsize=(6,6))
    plt.plot(x, y,label='Function')#在当前的对象上进行操作
    plt.grid(True) # 显示网格
    plt.axis("equal") # 设置了x、y刻度长度一致#需要放在x、ylim指令前
    plt.xlim(axisRange) # 显示的x的范围（不设置则由程序自动设置）
    plt.ylim(axisRange) # 显示的y的范围

    plt.plot([min(x),max(x)], [0,0],label='x-axis')
    plt.plot([0,0],[min(y),max(y)],label='y-axis')
    plt.legend() # 显示旁注，注意：不会显示后来再定义的旁注
    plt.show(fig) # 没有输入值默认展示所有对象
    # 注意：plt.show()之后再次使用plt.show()指令将不会展示任何对象，若想再次展示对象，可以对对象使用fig.show()
	