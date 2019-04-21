from matplotlib import pyplot as plt

# 参数依次为list,抬头,X轴标签,Y轴标签,XY轴的范围
def draw_hist(myList,Xrange,Yrange,Title="",Xlabel="",Ylabel=""):
    plt.hist(myList,len(myList))
    plt.xlabel(Xlabel)
    plt.xlim(Xrange[0],Xrange[1])
    plt.ylabel(Ylabel)
    plt.ylim(Yrange[0],Yrange[1])
    plt.title(Title)
    plt.show()
