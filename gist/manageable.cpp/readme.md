manageable.cpp
=========
可以自动回收对象的C++功能接口（即使被管理对象已经被释放了，回收阶段也不会炸）

快速使用
----------
* 让需要管理的类继承于`manageable`
* 在程序正式开始前调用`manageContain::init()`初始化管理器
* 调用`manageContain::reset()`触发回收，此时所有被管理对象（manageContain持有所有权）都会被释放
* 如果你不想让一个被管理的类的对象被回收，对其调用`obj->setSystemManage()`（使得manageContain不再对这个对象持有所有权）