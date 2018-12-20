import keras
from keras import backend as K
from keras.engine.topology import Layer
import numpy as np

class funlayer(Layer):
    def __init__(self, **kwargs):
        super(funlayer, self).__init__(**kwargs)

    def build(self, input_shape):
        assert len(input_shape)==2 and input_shape[-1]==3
        self.a = self.add_weight(name='a', shape=(3,),initializer='uniform',trainable=True)
        super(funlayer, self).build(input_shape)  # Be sure to call this somewhere!

    def call(self, x, **kwargs):
        '''' 流程张量操作的一个很好的演示
        b=[]
        b.append(self.a[0]*x[:,0])
        b.append(self.a[1]*x[:,1])
        b.append(self.a[2]*x[:,2])
        ret=K.stack(b,axis=1)
        '''
        ret=(self.a[0]*x[:,0])+(self.a[1]*x[:,1])+(self.a[2]*x[:,2])
        print(ret.shape)
        return ret

    def compute_output_shape(self, input_shape):
        return (input_shape[0],1) # 这里这个维度……

model=keras.Sequential()
model.add(funlayer())
opt=keras.optimizers.Adam(lr=0.01)
model.compile(optimizer=opt,loss='mse')

x=np.array([[1,1,1],[2,2,2],[3,3,3],[1,2,3],[3,5,7],[8,9,7],[4,5,6],[3,2,8],[4,1,2],[5,3,7],[4,8,2],[6,2,0],[0,0,0],[1,0,0],[1,2,0],[8,0,5]])
y=np.array([6,12,18,12,30,48,30,26,14,30,28,16,0,2,6,26])
model.fit(x, y, epochs=5000, batch_size=y.shape[0])
print(model.evaluate(x, y, batch_size=y.shape[0]))
x_test=np.array([[2,2,2],[8,7,10],[3,3,3]])
print(model.predict(x_test, batch_size=3))