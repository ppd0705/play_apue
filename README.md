
## 第一章 基础知识

### 1.2 体系结构
- 内核（kernel）： 控制计算机硬件资源，提空程序运行环境的软件
- 系统调用（system call）：内核的接口
- 公用函数库： 构建在系统调用上
- 应用程序： 既可使用公用函数库，也可使用系统调用
### 1.4 文件和目录
- 文件系统： 目录和文件的一种层次结构
- 目录： 一个包含目录项的文件，创建新目录是会自动创建两个文件名（. 和 ..）

### 1.5 输入和输出
- 文件描述符：用来标识一个特定进程正在访问的文件，通常是个非负整数
- 每当运行一个新程序时，shell都为其打开3个文件文件描述符，通常是0，1，2 -- 分别代表标准输入，标准输出，标准错误输出

### 1.6 程序和进程
- 程序：一个可执行文件，内核使用exec函数将程序读入内存，并执行程序
- 进程：程序执行的实例称之为进程，每一个进程有个唯一的数字标识符，称之为进程ID
- 进程控制：3个主要用于控制进程的函数：fork, exec, waitpid
- 线程：通常一个进程只有一个线程 -- 某一时刻执行的一组机器指令；进程内所有线程共享同一地址空间、文件描述符、栈和进程相关熟悉；每个线程也有一个标识ID, 这个ID只在该进程内起作用

### 1.7 出错处理
- 函数出错时，通常会返回一个负值，而且全局整形变量errno会设置为一个特定的值
- 函数strerror会返回errno对应消息字符串的指针
- 函数perror会将出错信息输出到标准错误输出
- 出错恢复： 对于非致命的错误，程序应该延迟一段时间然后重试，提高程序的健壮性

## 第二章 标准及实现
### 2.2 UNIX标准
- ISO C
- POSIX
### 2.3  UNIX系统实现
- BSD
- linux
- Mac O SX
### 2.5 限制
- 编译时限制：头文件中定义
- 运行时限制：调用其他函数获得（sysconf，pathconf, fpathconf）


## 第三章 文件I/O
### 3.2 文件描述符
- 一个非负整数，内对对于已打开的文件的引用表示
- 通常0，1，2分别表示标准输入，标准输出，标准错误输出
### 3.3 函数open和openat
参数
- path: `char *`
- oflag: `int` 多个常量使用或运算构成
    - O_RDONLY  只读
    - O_WRONLY 只写
    - O_RDWR 读写
    - O_EXEC 只执行
    - O_APPEND  追加写
    - O_CREATE 不存在则创建
    - O_NOBLOCK 非阻塞
    - O_TRUNC 将文件长度截为0
- fd: `int`
    - path使用绝对路径时fd被忽略，openat相当于open
    - path指定相对路径， fd指出了相对路径在文件系统中的开始地址
    - openat的作用是是的线程可以打开相对路径名打开文件，而非只限于当前工作目录
### 3.4 函数create
- 早期UNIX系统open函数不支持打开一个尚未存在的文件
- 等效于 open(path, O_WRONLY | o_CREATE | O_TRUNC, mode)
### 3.5 函数close
- 进程终止时，内核会自动关闭它所有的打开文件
- 关闭一个文件是会释放进程加载该文件上的所有记录锁

### 3.6 函数lseek
lseek不会引起任何IO操作，只是将当前偏移量记录在内核中
```C
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
```
- whence
    - SEEK_SET: 文件开始处
    - SEEK_CUR： 文件当前值
    - SEEK_END：文件结尾处
### 3.10 文件共享
使用三级数据结构表示打开的文件
- 进程表

|fd|文件指针|
|--|--|
|0| * |

- 文件表
    - 文件状态标志（读、写，是否阻塞）
    - 文件偏移量
    - 指向v节点的指针

- v节点表
  - 节点信息
  - 文件长度

注： 多个进程打开同一个文件，会有各自的进程表和文件表，但指向同一个v节点表

### 3.11 原子操作
任何要求多余一个函数调用的操作都不是原子操作

### 3.12 函数dup和dup2
```C
#include <unistd.h>
int dup(int fd);
int dup2(int fd, int fd2);
```
dup函数用来复制一个现有的文描述符，新的fd与原有的fd共享文件表

### 3.13 函数sync, fsync

```C
#include <unistd.h>
void sync(void);
int fsync(int fd);
```
- 延迟写（delayed write）: 向文件写入数据是，内存通常现将数据复制到缓冲区，中，然后排入队列
- sync: 将所有修改过的块缓冲区排入写队列，不等待实际磁盘写操作结束就返回
- fsync: 只对fd指定的文件起作用，等待写操作结束才返回，除更新数据外，也同步更新文件的属性

### 3.14 函数fcntl
```C
#include <fcntl.h>
int fcntl(int fd, int cmd, .../* int arg */);
```
fcntl可以改变已经打开的文件的属性，主要有以下5种功能
- 复制一个已有的描述符（cmd = F_DUPDF or F_DUPFD_CLOEXEC）
- 获取或设置文件描述符标志(F_GETFD or F_SETFD)
- 获取或设置文件状态标志(F_GETFL or F_SETFL)
- 获取或设置异步IO所有权(F_GETOWN or F_SETOWN)
- 记录或设置记录锁(F_GETLK or F_SETLK or F_SETLKW)

## 第四章 文件和目录

### 4.3 文件类型
- 普通文件（regular file）
- 目录文件（directory file）
- 块特殊文件（block special file）对设备进行带缓冲的访问，每次访问的长度固定
- 字符特殊文件（character special file） 对设备访问不带访问，访问长度可变
- FIFO (named pipe)
- 套接字（socket）
- 符号链接（symbolic link

### 4.5 文件访问权限
- 每个文件有9个访问权限位（用户写读执行，组写读执行，其他写读执行）
- 打开任意类型的文件时，需要对文件所在的各级目录均有执行权限
- 创建或删除一个文件时，需要对文件的目录有执行权限和写权限

### 4.8 umask
umask函数为进程设置文件模式创建屏蔽字，并返回之前的值
umask的值是027时代表阻止同组成员写文件以及其他用户读写执行文件

### 4.9 chmod、fchmod、fchmodat
chmod指在指定的文件上进行操作
fchmod指对已经打开的文件进行操作


## 第五章 标准I/O库
### 5.2 流和FILE对象
- 当使用标准I/O库打开或创建一个文件时，我们使用一个流和一个文件相关联
- 流的字符集分为单字节或者多字节的，对应的流程对象为字节定向或宽定向
- `freopen`会清除一个流的定向，`fwide`可以获取流定向的信息
- FILE对象包含了管理流需要的所有信息
  - 文件描述符
  - 指向流缓冲区的指针
  - 缓冲区的长度
  - 当前缓冲区的字符数
  - 出错标志

### 5.4 缓冲
- 三种类型
  - 全缓冲：填满缓冲区后才进行实际的I/O操作
  - 行缓冲：遇到换行符时执行I/O操作
  - 不带缓冲
— UNIX惯例：标准错误不带缓冲，终端设备的流为行缓冲，其他流为全缓冲
- 可通过`setbuf`或`setvbuf`设置缓冲类型
  - _IOFBF 全缓冲
  - _IOLBF 行缓冲
  - _IONBF 无缓冲
- `fflush` 会将缓冲区的数据传送至内核

### 5.5 打开流
`fopen`、 `freopen`

### 5.6 每次一个字符的IO
- 一次读一个字符可用如下三个函数
  - getc(FILE *fp)
  - fgetc(FILE *fp)
  - getchar == getchar(stdin)
- 压回字节流： ungetc(int c, FILE *fp)
- 输出: putc, fputc, putchar

### 5.7 每次一行的IO

- fgets/fputs: 缓冲区总以null结尾

### 5.9 二进制IO
- 一次读一个完成的结构，不会像fgets遇到null就会停止
- 二进制IO跨系统可能会遇到不兼容的问题，如浮点数的格式，结构的对齐方式可能不同
### 5.10 定位流

### 5.11 格式化IO
#### 输出
- printf: 格式化数据写到标准输出
- fprintf： 写到指定的流
- dprintf： 写到指定的文件描述符
- sprintf： 写到指定的数组buf中， 可能会有溢出问题
- snprinf： 写到指定的buf中，溢出的数据会被丢弃

#### 输入
- scanf
- fscanf
- sscanf

### 5.13 临时文件
- tmpnam: 返回一个有效路径名的字符串
- tmpfile：通常的实现是先调用tmpmam产生一个唯一的路径名，然后用该路径名创建一个文件并立即unlink它，这样在关闭该文件或程序结束时将自动删除这种文件；不过正因为分为两步，有可能另外一进程也用相同的名字创建文件
- mkstemp: 创建一个普通文件含读写权限，若要自动删除需要主动unlink

### 5.14 内存流
fmemopen: 可以指定缓冲区用户内存流

## 第六章 系统数据文件和信息

### 6.2 密码文件
- 文件名 /etc/passwd
- 存储struct： passwd
- 查看用户信息 finger -p username
### 6.3 阴影密码
- 文件名：/etc/shadow
- struct: spwd

### 6.4 组文件
- 文件： /etc/group

### 6.10 时间和日期

## 第七章 进程环境

## 第八章 进程控制

## 第九章 进程关系

## 第十章 信号

## 第十一章 线程

## 第十二章 线程控制

## 第十三章 守护进程

## 第十四章 高级I/O

## 第十五章 进程间通信

## 第十六章 套接字

## 第十七章 高级进程间通信

## 第十八章 终端I/O

## 第十九章 伪终端

## 第二十章 数据库函数

## 第二十一章 与网络打印机通信

### 参考

- [源码下载](http://www.apuebook.com/code3e.html)
- [Make and apply apue.h](https://stackoverflow.com/questions/53236356/make-and-apply-apue-h)
- [errorno详解](http://c.biancheng.net/c/errno/)
- [huaxz1986/APUE_notes](https://github.com/huaxz1986/APUE_notes/)