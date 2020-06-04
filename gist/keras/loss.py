from keras import backend as K

def IoU(y_true, y_pred):
    # calculate equality of the predictions and truths to the label
    # calculate the |intersection| (AND) of the labels
    intersection = K.sum(y_true * y_pred)
    # calculate the |union| (OR) of the labels
    union = K.sum(y_true) + K.sum(y_pred) - intersection
    # avoid divide by zero - if the union is zero, return 1
    # otherwise, return the intersection over union
    return K.switch(K.equal(union, 0), 1.0, intersection / union)


def tversky_loss(y_true, y_pred):
    # 这俩参数能调，aplha是多惩罚假阳性，beta是多惩罚假阴性
    alpha = 0.5
    beta = 1-alpha

    ones = K.ones(K.shape(y_true))
    p0 = y_pred  # proba that voxels are class i
    p1 = ones - y_pred  # proba that voxels are not class i
    g0 = y_true
    g1 = ones - y_true

    num = K.sum(p0 * g0)
    den = num + alpha * K.sum(p0 * g1) + beta * K.sum(p1 * g0)

    T = K.sum(num / den)  # when summing over classes, T has dynamic range [0 Ncl]
    Ncl = K.cast(K.shape(y_true)[-1], 'float32')
    return Ncl - T

def focal_loss(y_true, y_pred):
    # gamma调整损失衰减，准确率越高的样本衰减越多，越低的衰减的越少，这样整个loss就是由准确率较低的样本主导了
    # alpha是正负样本权重，当class为1的时候，乘以权重alpha，当class为0的时候，乘以权重1-alpha
    alpha=0.25 # 之所以这里给正例样本调低是因为，正例少，准确度低，所以本身就会被gamma加强。这里给正例降权是为了平衡gamma加权的影响
    gamma=2

    pt_1 = tf.where(tf.equal(y_true, 1), y_pred, tf.ones_like(y_pred))
    pt_0 = tf.where(tf.equal(y_true, 0), y_pred, tf.zeros_like(y_pred))
    return -K.sum(alpha * K.pow(1. - pt_1, gamma) * K.log(pt_1))-K.sum((1-alpha) * K.pow( pt_0, gamma) * K.log(1. - pt_0))
