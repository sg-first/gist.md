import numhelp

def entropy():
	num,dim = data.shape
	numhelp.normalization(data) # 归一化
	dimsum=data.sum(axis=0) # 不同维度所有数据的和
	dataP=data/dimsum # 各个维度除和，创建权重矩阵
	k=1/np.log(num)
	# 计算熵值
	dataE=dataP*np.log(dataP) # 此时仍为二维
	#dataE=np.nan_to_num(dataE) # 看数据再加这个
	dataE=-k*dataE.sum(axis=0) # 变为一维
	# 计算信息熵冗余度
	dataD=1-dataE
	#计算各个指标（维度）权值
	return dataD/dataD.sum(axis=0)