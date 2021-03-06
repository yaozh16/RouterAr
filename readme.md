# 路由器转发引擎实现细节：
## 路由表方面：
### 概述
* 整体使用了一个二叉树写法（压缩掉多余节点的线段树）
    * 要求：若B对应的路由子网属于A对应的路由子网的子网，则B对应树节点必须是A的路由节点的后代节点
### 插入
* 对于新插入的节点A，首先进行搜索，判断：
    * 恰好查到掩码和一样的节点：直接设置该节点的有效标志位为1
    * 否则，若A应该是某个已有节点B的父节点，则
        * 在B的父节点C与B之间插入A
        * 将B标记为A的子节点
        * 将A标记为C的子节点
    * 否则，若A应该是某个已有节点B的子节点，则
        * B有子节点，继续下溯查找
        * B无子节点，直接插在B的子节点
    * 如果A是节点B的子节点，但是A与节点B的直接子节点C、D都不相干：
        * 例如
            * A:```192.168.1.0/24```
            * B:```192.168.0.0/22```
            * C:```192.168.0.0/24```
            * D:```192.168.2.0/24```
        * 则尝试merge三个节点(A\C\D)中的两个使得新节点的共同最小父节点（掩码最长）
            * 上面例子中产生的新的merge节点为
                * M:```192.168.0.0/23```，为A、C共同最小父节点
        * 新merge的节点插入B占据一个子节点位置、剩下的一个节点占据B的另一个子节点
            * 上面例子中最后：
                * B有子节点M、D
                * M有子节点C、A
                * M节点的有效位为0
### 删除：
* 执行完全匹配直接查找，查找中保存一个最大32层的栈，保存查找命中下溯记录
* 如果找到完全匹配删除路由的节点:
    * 如果不是叶子节点，直接设置有效标志位为0
    * 如果是叶子节点，则删除该节点，同时根据命中栈，一层一层上溯:
        * 如果上一层的节点的子节点都是空（变为了叶子），且节点有效位为0,则删除该节点，并继续上溯
        * 否则直接返回
### 查找：
* 执行最长匹配原则，一路下溯尝试匹配，保存最新命中位置，在命中位置继续下溯直至不存在前缀匹配的子节点
* 如果命中位置非空，则按照最新命中位置进行返回，否则返回错误
* 命中的要求：前缀匹配，且节点有效标志位为1
* 对于前缀匹配得上但是有效位为0的节点，仍然执行下溯，但是不命中保存
## 线程安全方面
* 采用了一个pthread_rwlock_t读写锁，保证安全的同时,
尽可能允许同时进行查询操作
* 程序初始化时候进行init
* 插入删除路由进行wrlock（退出时unlock）
* 查找路由进行rdlock（退出时unlock），支持并行查找