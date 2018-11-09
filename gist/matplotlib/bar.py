import numpy as np
import matplotlib.pylab as pylab
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick

params={
    'axes.labelsize': '35',
    'xtick.labelsize':'27',
    'ytick.labelsize':'27',
    'lines.linewidth':2 ,
    'legend.fontsize': '27',
    'figure.figsize': '24, 9'
}
pylab.rcParams.update(params)

# 定义一样的条目每一组的高度
y1 = [9.79,7.25,7.24,4.78,1] # 所有蓝色条目的高度
y2 = [5.88,4.55,4.25,3.78,3.92]
y3 = [4.69,4.04,3.84,3.85,4.0]
y4 = [4.45,3.96,3.82,3.80,3.79]
y5 = [3.82,3.89,3.89,3.78,3.77]

ind = np.arange(5) # 组数
width = 0.15
plt.bar(ind,y1,width,color = 'blue',label = 'm=2')
plt.bar(ind+1*width,y2,width,color = 'g',label = 'm=4') # ind+width 调整条形图的左起始位置,高度（每一组）,宽度
plt.bar(ind+2*width,y3,width,color = 'c',label = 'm=6')
plt.bar(ind+3*width,y4,width,color = 'r',label = 'm=8')
plt.bar(ind+4*width,y5,width,color = 'm',label = 'm=10')
plt.xticks(np.arange(5) + 2.5*width, ('10%','15%','20%','25%','30%'))

plt.xlabel('Sample percentage')
plt.ylabel('Error rate')

fmt = '%.0f%%' # Format you want the ticks, e.g. '40%'
xticks = mtick.FormatStrFormatter(fmt)
# Set the formatter
axes = plt.gca() # get current axes
axes.yaxis.set_major_formatter(xticks) # set % format to ystick.
axes.grid(True)
plt.legend(loc="upper right")
#plt.savefig('D:\\errorRate.eps', format='eps',dpi = 1000,bbox_inches='tight')

plt.show()