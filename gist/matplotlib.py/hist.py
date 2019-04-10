from matplotlib import pyplot as plt

# 参数依次为list,抬头,X轴标签,Y轴标签,XY轴的范围
def draw_hist(myList,Title="",Xlabel="",Ylabel="",Xrange,Yrange):
    plt.hist(myList,100)
    plt.xlabel(Xlabel)
    plt.xlim(Xrange[0],Xrange[1])
    plt.ylabel(Ylabel)
    plt.ylim(Yrange[0],Yrange[1])
    plt.title(Title)
    plt.show()
