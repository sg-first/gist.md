import os, shutil
import os.path;
import codecs;
import stat;

# 获取目录下所有后缀为postfix的文件名
def GetFilenames(path, postfix):
    return [file for _,_,f in os.walk(path) for file in f if file.endswith(postfix)]

# 获取目录下后缀为postfix的文件
def GetFiles(path, postfix):
    return [os.path.join(r,file) for r,d,f in os.walk(path) for file in f if file.endswith(postfix)]
    
# 获取目录下所有的文件
def GetAllFiles(path):
    return [os.path.join(r,file) for r,d,f in os.walk(path) for file in f]

# 获取目录下的子目录
def GetSubDirs(path):
    return [os.path.join(r,dir) for r,d,f in os.walk(path) for dir in d]

# 将string array写入文件
def WriteAllLines(fileName, lines):
    with codecs.open(fileName, 'w', "utf-8") as f:
        f.write("\n".join(lines));

#读取文件内容到string array
def ReadAllLines(fileName):
    f = codecs.open(fileName,'r','utf-8') 
    content = f.readlines()
    f.close()
    return content

# 设置文件夹制度
def MakeFileWriteableInDir(path):
    files = GetAllFiles(path);
    for file in files:
        os.chmod(file, stat.S_IWRITE)

#设置文件只读
def MakeFileWriteable(path):
    os.chmod(path, stat.S_IWRITE)

def GetFileNameWithoutExt(path):
    return os.path.splitext(os.path.basename(path))[0]

#删除目录下的所有文件
def DeleteDirFiles(path):
    files = GetAllFiles(path);
    for file in files:
        os.remove(file)

# 拷贝文件夹中的所有文件
def CopyTree(src, dst, symlinks = False, ignore = None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)

# 更新文件夹中的文件
def UpdateDir(root_src_dir, root_dst_dir):
    for src_dir, dirs, files in os.walk(root_src_dir):
        dst_dir = src_dir.replace(root_src_dir, root_dst_dir, 1)
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
        for file_ in files:
            src_file = os.path.join(src_dir, file_)
            dst_file = os.path.join(dst_dir, file_)
            if os.path.exists(dst_file):
                # in case of the src and dst are the same file
                if os.path.samefile(src_file, dst_file):
                    continue
                os.remove(dst_file)
            shutil.move(src_file, dst_dir)