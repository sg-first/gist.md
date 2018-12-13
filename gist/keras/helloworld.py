import keras
import numpy as np

print("hello world")

model=keras.Sequential() # 创建序贯模型神经网络，下面可以用堆叠的方法加入不同的层
model.add(keras.layers.Dense(2, input_shape=(1,),activation='relu')) # 添加全连接层
model.add(keras.layers.Dense(1, input_shape=(2,),activation='relu')) # 添加全连接层
optimizer = keras.optimizers.SGD(lr=0.01, momentum=0)
model.compile(optimizer=optimizer,loss='mse') # 模型设定完成，编译网络，使用rmsprop作为优化算法，mse作为损失函数
x=np.array([0,1,2,3,4,5,6,7,8])
y=np.array([1,2,3,4,5,6,7,8,9])
model.fit(x, y, epochs=1000, batch_size=10) # 用数据训练模型（函数y=x+1）
model.load_weights('my_model_weights.h5') # 保存权重的函数为model.save_weights('my_model_weights.h5')
print(model.evaluate(x, y, batch_size=10)) # 训练结束，评估模型
x_test=np.array([1,3,5,7,9,11,13])
print(model.predict(x_test, batch_size=7))