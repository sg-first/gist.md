from Utils import FileUtils
from PIL import Image
from numpy import average, dot, linalg

# 对图片进行统一化处理
def get_thum(image, size=(64, 64), greyscale=False):
    # 利用image对图像大小重新设置, Image.ANTIALIAS为高质量的
    image = image.resize(size, Image.ANTIALIAS)
    if greyscale:
        # 将图片转换为L模式，其为灰度图，其每个像素用8个bit表示
        image = image.convert('L')
    return image

# 计算图片的余弦距离
def image_similarity_vectors_via_numpy(image1, image2):
    image1 = get_thum(image1)
    image2 = get_thum(image2)
    images = [image1, image2]
    vectors = []
    norms = []
    for image in images:
        vector = []
        for pixel_tuple in image.getdata():
            vector.append(average(pixel_tuple))
        vectors.append(vector)
        # 求图片的范数
        norms.append(linalg.norm(vector, 2))
    a, b = vectors
    a_norm, b_norm = norms
    res = dot(a / a_norm, b / b_norm)
    return res

def detectFoladFolder(path):
    allTexture = FileUtils.GetFilenames(path, '.TGA')
    detected = set()
    for i in allTexture:
        if not i in detected:
            image1 = Image.open(path + i)
            for j in allTexture:
                if (not j in detected) and i != j:
                    image2 = Image.open(path + j)
                    if image1.size == image2.size:
                        cosin = image_similarity_vectors_via_numpy(image1, image2)
                        if cosin > 0.97:
                            print(i, j, cosin)
                            detected.add(i)
                            detected.add(j)
    print(path, 'finished')

if __name__ == "__main__":
    detectFoladFolder('D:/texture/verysmall/')
    detectFoladFolder('D:/texture/small/')
    detectFoladFolder('D:/texture/big/')