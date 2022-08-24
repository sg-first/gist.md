from Utils import FileUtils
from PIL import Image
from numpy import average, dot, linalg

# 对图片进行统一化处理
def get_thum(image, size=(64, 64), greyscale=False):
    # 利用image对图像大小重新设置, Image.ANTIALIAS为高质量的
    img2 = image.resize(size, Image.ANTIALIAS)
    if greyscale:
        # 将图片转换为L模式，其为灰度图，其每个像素用8个bit表示
        img2 = img2.convert('L')
    return img2

def get_norm(image):
    vector = []
    for pixel_tuple in image.getdata():
        vector.append(average(pixel_tuple))
    return vector / linalg.norm(vector, 2)
    
class img:
    def __init__(self, filename):
        self.image = Image.open(filename)
        self.normalizedNorm = get_norm(get_thum(self.image))

def loadAll(allTexture, path):
    ret={}
    for i in allTexture:
        ret[i] = img(path + i)
    print('load finished')
    return ret

def detectFoladFolder(path):
    allTexture = FileUtils.GetFilenames(path, '.TGA')
    detected = set()
    textureMap = loadAll(allTexture, path)
    
    for i, image1 in textureMap.items():
        if not i in detected:
            for j, image2 in textureMap.items():
                if (not j in detected) and i != j:
                    if image1.image.size == image2.image.size:
                        cosin = dot(image1.normalizedNorm, image2.normalizedNorm)
                        if cosin > 0.97:
                            print(i, j, cosin)
                            detected.add(i)
                            detected.add(j)
    print(path, 'finished')

if __name__ == "__main__":
    detectFoladFolder('D:/texture/small/')
    detectFoladFolder('D:/texture/big/')
