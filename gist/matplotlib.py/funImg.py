import numpy as np
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

def funImg2D(f, step=0.03, axisRange=(-10,10)):
    # 计算所有的点
    num=(axisRange[1]-axisRange[0])/step
    x = np.linspace(axisRange[0],axisRange[1],num)
    y = f(x)

    fig=plt.figure()
    plt.plot(x, y) # 在当前的对象上进行操作
    plt.grid(True) # 显示网格
    # plt.axis("equal") # 设置了x、y刻度长度一致
    plt.show(fig) # 没有输入值默认展示所有对象
    # 注意：plt.show()之后再次使用plt.show()指令将不会展示任何对象，若想再次展示对象，可以对对象使用fig.show()

def funImg3D(f, step=0.03, axisRange=(-10,10)):
    x=np.arange(axisRange[0],axisRange[1],step)
    y=np.arange(axisRange[0],axisRange[1],step)
    x,y=np.meshgrid(x,y)
    z=f(x,y)

    fig = plt.figure()
    ax = Axes3D(fig)
    ax.plot_surface(x,y,z,cmap='rainbow')
    plt.show(fig)
