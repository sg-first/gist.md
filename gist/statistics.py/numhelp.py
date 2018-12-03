import numpy as np

def normalization(array):
    maxcols=array.max(axis=0)
    mincols=array.min(axis=0)
    data_rows,data_cols = array.shape
    t=np.zeros((data_rows,data_cols))
    for i in range(data_cols):
        t[:,i]=(array[:,i]-mincols[i])/(maxcols[i]-mincols[i])
    return t