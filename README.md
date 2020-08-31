
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

### 7.3 进程终止

#### 终止方式

- 正常终止
  - main返回
  - exit
  - _exit
  - 最后一个线程从其启动例程返回
  - 最后一个线程条用pthread_exit
- 异常终止
  - 调用abort
  - 接到一个信号
  - 最后一个线程对取消请求做响应
  
#### 退出
- _exit会立即进入内核，而exit会执行一下清除处理如fclose，再返回内核
- atexit
```C
#include <stdlib.h>
int atexit(void (*func)(void));
```
atexit用来登记终止处理程序(exit handler)，exit执行时会自动调用，调用的顺序和登记顺序相反
 
### 7.4 命令行参数
```C
int main(int argc, char *argv[])
```
`argv[argc]`是个空指针

### 7.5 环境表
```C
extern char **environ;
```
全局变量environ为指向环境表的指针，环境表为一个字符串指针数组，字符串为`key-value`形式

### 7.6 C程序的存储空间布局
- 正文段：CPU执行的机器指令部分，通常可同享和只读
- 初始数据段：明确需要赋初值的变量
- 未初始化的数据段(bss)：通常初始化为0或空指针
- 堆：通常在堆中动态存储分配
- 栈：函数调用的调用者的环境信息和返回结果的地址，及局部变量信息
- 命令行参数和环境变量

### 7.7 共享库
共享库使得可执行文件中并在需要宝行公用的库函数，程序在第一次执行或者第一只调用某个库函数的时候，用动态链接的方法将程序与共享库函数相链接

### 7.8 存储空间分配

#### 三个函数
```C
#include <stdlib.h>
void *malloc(size_t size);
void *calloc(size_t nobj, size_t szie);
void *realloc(void *ptr, size_t newsize);
```
- malloc: 分配制度字节数的存储区，初始值不确定
- calloc: 为指定数量指定长度的对象分配存储空间，每一个bit都初始化为0
- reallloc: 增加或减少之前分配区的长度。增加长度时，可能需要将之前分配区的内容复制到
新的够大的区域，新增区域的初始值不确定


### 7.9 环境变量
```C
#include <stdlib.h>

int putenv(char *str);
int setent(const char *name, const char *value, int rewrite)
int unsetenv(const char *name);
```
- putenv取`name=value`格式的字符串放到环境表中，如果name已存在，则先删除之前的定义
- setenv将name设置为value, 如果name已经存在，若rewrite非0，则先删除现有定义，若rewrite=0则不更新
- unsetenv删除name的定义

环境变量的更新

- 默认环境表和指向的环境字符串都存在栈之上
- 删除找到对应的指针，后面的纸质依次向前移动一个位置即可
- 如果更新一个name，若value长度小于等于原值，原地更新；否则得malloc分配空间
- 如果新增一个name，则需要malloc为环境表和新怎的字符串分配空间，不过环境表的指针
大部分仍指向栈顶指向的空间

### 7.10 setjump和longjump函数
```C
#include <setjump.h>
int setjmp(jump_buf env);
void longjmp(jmp_buf emv, int val);
```
- longjump实现了非本地跳转机制，常用于信号处理和异常处理
- wiki: https://zh.wikipedia.org/wiki/Setjmp.h


### 7.11 函数getrlimit和setrlimit

进程资源限制的查询和更改
```C
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit *rlprt);
init setrlimit(int resource, const struct rlimit *rlprt);

struct rlimit {
    rlimi_t rlimi_curr; // soft limit
    rlimi_t rlimi_max; // hard limit: maxinum value for rlimi_cur
}
```
- 更改资源限制的规则
  - 任何一个进程都可以将软限制值改为小于等于其硬限制值
  - 任何一个进程都可以降低其硬限制值
  - 只有超级用户进程可以提高硬限制值
  
- 资源种类
  - RLIMIT_AS: 进程可用存储空间的最大字节
  - RLIMIT_DATA: 数据段的最大字节长度（初始化数据、非初始化数据和堆的总和）
  - RLIMIT_NOFILE: 进程能打开的最多文件数 
  - RLIMIT_STACK: 栈的最大字节长度 
  
  
## 第八章 进程控制

### 8.2 进程标识
- ID为0的进程为调度进程，为内核的一部分
- ID为1的进程为init进程，以超级用户权限运行
- ID为2的进程为页守护进程(page daemon)，负责支持虚拟存储器系统的分页操作

### 8.3 fork函数
```C
#include <unistd.h>
pid_t fork(void);
```
fork函数用于创建一个新的进程，
- 调用一次返回两次，子进程的返回值是0，父进程的返回值是子进程的pid
- 子进程获得父进程的数据空间，堆和栈的副本，使用copy-on-write技术，这些区域访问权限改为只读，
如果父进程或子进程修改了部分区域，那内核只为修改的那块内存制作一个副本，通常是虚拟内存中的一页

### 8.5 exit
#### 子进程终止
- 当进程正常终止时，exit或_exit函数的返回值的返回值作为终止状态(termination status);
- 异常终止时，内核会产生一个指示异常终止原因的终止状态
- 父进程可以用wait或waitpid获取子进程的终止状态
- 父进程未对其善后处理（获取终止状态、是否仍占用的资源）的进程称为zombie僵尸进程

#### 父进程终止
 - 父进程终止后，子进程的父进程变成init进程，init进程只有子进程终止，就会调用wait,这样就不会导致系统塞满了僵尸进程
 
#### 8.6 wait和waitpid函数
```C
#include <sys/wait.h>

pid_t wait(int *statloc);
pid_t waitpid(pid_t pid,int *statloc, int options);
```
- 当一个进程结束时，内核会向其父进程发送`SIHCHLD`信号，父进程可以选择忽略（默认），或者提供一个响应函数
- wait: 如果所有子进程都在运行则阻塞，有子进程终止则获得子进程的终止状态立即返回，如果没有子进程会出错
- waitpid与wait的区别
  - 可以指定等待的pid
    - pid == -1: 等待任一子进程，等效于wait
    - pid == 0: 等待组ID和调用进程的组ID相同的子进程
    - pid >0 : 等待指定子进程
    - pid < -1: 等待组ID等于abs(pid)的任一子进程
  - waitpid可以设置option
    - WNOHANG: 如果指定的至今传并不是立即可用的，waitpid不阻塞，返回0
    - WCONTINUED
    - WUNTRACED
        
- statloc会返回终止状态

- 通过四个互斥的宏可以获得进程终止的原因
  - WIFEXITED(status): 如果true表示正常终止
  - WIFSIGNALED(status): 如果true表示异常终止
  - WIFSTOPPED(status): 如果true表示暂停
  - WIFCONTINUED(status): 如果true表示暂停后又继续

#### 8.7 waitid函数
```C
#include <sys/wait.h>
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

- idtype
  - P_PID: 等待特定子进程
  - P_PGID: 等待特定进程组的子进程
  - P_ALL: 等待任一子进程
- options
  - WCONTINUED: 子进程曾被停止后又已继续
  - WEXITED: 已退出
  - WNOHANG:  如无可用的子进程退出状态，立即返回而非阻塞
  - WNOWAIT: 不破坏子进程的退出状态，仍可用wait/waitid/waitpid获取
  - WSTOPPED: 已停止的子进程
 
#### 8.8 wait3和wait4函数
相比wait函数，可以获取子进程的使用资源概况
```C
#include <sys/type.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

pid_t wait3(int *statloc, int options, struct rusage *rusage)
pid_t wait4(pid_t pid,int *statloc, int options, struct rusage *rusage)
```

#### 8.9 竞争条件（race condition）
如果多个进程对企图对共享数据进行某种处理，而最后的结果又取决于进程运行的顺序时，我们认为发生了race condition

#### 8.10 exec函数

exec函数只是用磁盘上的一个进程序替换了当前进程的正文段，数据段，堆和栈

```C
#include <unitstd.h>
int execl (const char *pathname, const char *arg0, ... /* (char *)0 */);
int execv (const char *pathname, char *const argv[]);
int execle (const char *pathname, const char *arg0, ... /* (char *)0, char *const envp[] */);
int execve (const char *pathname, char *const argv[], char *const envp[]);
int execlp (const char *filename, const char *arg0, ... /* (char *)0 */);
int execvp (const char *filename, char *const argv[]);
int fexecve(int fd, char *const argv[], char *const envp[]);
```

- 第一个参数前面四个函数取路径名作为参数，后面两个取文件名作为参数，最后一个取fd作为参数

- 第二个区别是参数表的传递，execl/execlp/execle要求将新程序的每个命令行参数都说明为一个单独的参数，这种参数表以空指针结尾；
对另外四个参数则应该构建一个指向各参数的指针数组，然后将该数组地址作为参数

- 在很多UNIX实现中，只有execve是内核的系统调用，另外6个函数最终都要调用该系统调用

#### 8.11 更改用户ID和更改组ID
```C
#include <unistd.h>

int setuid(uid_t uid);
int setgid(gid_t gid);
```

#### 8.12 解释器文件(interpreter file)

通常的形式是`#! pathname [optional-argument]`，!和pathname之间的空格可选

#### 8.13 函数system
```C
#include <stdlib.h>
int system(const char *cmdstring);
```
`system`在实现中调用了`fork`, `exec`, `waitpid`

#### 8.14 进程会计process accounting
每当进程结束时内核就会写一个会计记录，一般包括命令名、CPU总时间、用户ID、启动时间等，各个平台有所区别
- 不能获取init进程这种永不终止的进程的记录
- 记录的顺序对应进程终止的顺序


#### 8.16 进程调度

##### nice进程调度优先级，nice值越高，优先级越低， 可选范围在`0~(2*NZERO)-1`

- `nice` 设置当前进程的优先级

```c
#include <unistd.h>

//return: 若成功，返回新的nice值，否则返回-1
int nice(int incr);
```

- `getpriority`获取进程或进程组的nice值

```c
#include <sys/resource.h>

int getpriority(int which, id_t who);
```

- `setpriority`为进程、进程组和特定用户的所有进程设置nice值

```c
#include <sys/resource.h>

int setpriority(int which, id_t who, int value);
```

#### 8.17 进程时间
```C
#include <sys/times.h>

struct tms {
    clock_t tms_utime; /* usert CPU time */
    clock_t tms_stime; /* system CPU time */
    clock_t tms_cutime; /* user CPU time, terminated chhildren */
    clock_t tms_cstime; /* system CPU time, terminated children */
}

clck_t times(struct tms *buf);
```

## 第九章 进程关系

### 9.2 终端登录
启动过程：init进程fork一个子进程 -> exec getty -> exec login

### 9.3 网络登录
系统使用伪终端(pseudo termiaml), 将终端的操作映射为网络操作 

### 9.4 进程组
每个进程都属于某个进程组

同一进程组的歌进程接受来自同一个终端的信号

```c
#include <unistd.h>

pid_t getpgrp(void);
```

每个进程组有一个组长进程，组长进程的进程ID和进程组ID相同

`setpgid`可以加入或创建一个新进程组
```c
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid);
```

### 9.5 会话（session）
session是一个或多个进程组的集合

`setsid`创建一个新的会话
```c
#include <unistd.h>
pid_t setsid(void);
```


### 9.6 控制终端
一个会话可以有一个控制终端（control terminal）

与控制终端连接的进程称为控制进程（controlling process）

一个会话通常有一个前台进程组(foreground process group)和一个或多个后台进程组(background process group)

终端输入和终端产生的信号会发送至前台进程组


## 第十章 信号

### 10.3 signal函数
```c
#include <signal.h>

void (*signal(int signo, void (*func)(int))) (int);
```
- 执行一个程序时， 所有信号都是系统默认或忽略
- 子进程西城父进程的信号处理方式，因为子进程复制了父进程的内存映像

### 10.5 中断的系统调用

- 进程执行一个低速系统调用，在阻塞期间捕捉到洗衣歌信号，则该系统调用被中断不再继续执行，并将errno设为`EINTR`
- 低速系统调用指的是可能会使进程永远阻塞的一类系统调用
  - 读某些不存在的文件(如管道、终端设备、网络设备）
  - 写数据到某些文件,而文件没有立即接受
  - pause函数和wait函数
  - 某些ioctl操作
  - 某些进程间通信
  
- BSD4.2引进了自动重新启动功能，支持的踢动调用包括ioctl, read, wirte, wait
- POSIX要求中断的信号的SA_RESTART标志有效时，才重启该系统调用

### 10.6 可重入函数
- 信号捕捉到后
  - 正在执行的正常指令被中断
  - 执行信号处理程序
  - 如果没有exit或longjump则继续执行正常指令
  
- 信号处理程序中保证调用安全的函数，是可重入的（可恢复正常指令的执行），被称为异步信号安全的（async signal sage),这些函数有如下特定
  - 没有使用静态数据结构
  - 没有调用malloc或free
  - 没有调用标准IO函数，因为标准IO函数很多都使用了全局数据结构

- 信号处理程序应该保存调用之前的errno,调用后再恢复errno


### kill和raise函数
```c
#include <signal.h>
int kill(pid_t pid, int signo);
int raise(int signo);
```
`raise(signo)`等效于`kill(getpid(),signo)`


kill中的pid
- pid >0: 发送给指定pid进程
- pid == 0: 发送同一进程组的所有进程
- pid < -1: 发送给abs(pid)的进程组
- pid == -1: 发送给该进程有权限发信号的所有进程

signo=0定义为空信号，常用作确定一个进程是否任然存在

### 10.10 函数alarm和pause
```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```
调用alarm函数会设置定时器，超时会发送一个SIGALM信号，默认的动作是终止进程

```c
#include <unistd.H>

int pause(void);
```

pause函数使进程挂起直到捕捉到一个信号，在执行了信号处理程序化，pause才返回


### 10.11 信号集signal set
信号集： 一个能表示多个信号的数据类型

```c
#include <signal.h>

int sigemptyset(sigset_t *set)

int sigfillset(sigset_t *set)

int sigaddset(sigset_t *set, int signo);

int sigdelset(sigset_t *set, int signo);

int sigismember(sigset_t *set, int signo);
```

`sigemptyset` 初始化信号集，清除所有信号

`sigfillset` 初始化信号集，包含所有信号

如果只有实现的信号数小于一个整型位数，可以用一位代表一个信号

```c
#define sigemptyset(ptr) (*(prt) = 0)

# 使用逗号运算符，这样sigfillset会返回0
#define sigfillset(ptr) (*(prt) = ~(sigset)0, 0)

```

### 10.12 函数sigprocmask

进程信号屏蔽设置

```C
# include <signal.h>

int sigprocmask(int how, const, sigset_t *restrict set, sigset_t *restrict oset);
```
- how: SIG_BLOCK, 或添加set; SIG_UNBLOCK, 接触set中的信号； SIG_SETMASK: 赋值为set中的信号
- set: how操作的信号集， set为null时how没有意义
- oset: 返回当前信号屏蔽字

### 10.13  函数sigpending

返回当前阻塞的信号集
```C
#include <signal.h>

int sigpending(sigset_t *set);
```

### 10.14 函数sigaction
检查或修改指定信号的handler

```C
#include <signal.h>

int sigaction(int signo, const struct sigaction *resitrct act, struct sigaction *restrict oact);


struct sigaction {

void  (*sa_handler)(int);
sigset_t sa_mask;
int sa_flags;
void (*sa_sigaction) (int, siginfo_t *, void *);
}
```

### 10.15 函数sigsetjmp和siglongjmp
```C
#include <setjmp.h>
int sigsetjmp(sigjmp_buf env, int savemask);

void siglongjmp(sigjmp_buf env, int val);

```
和setjmp/longjmp唯一的区别是savemask非0时，会在env中保存进程当前的信号屏蔽字，siglongjmp从
其中恢复保存的信号屏蔽字

### 10.16 函数sigsuspend
```C
#include <signal.h>

int sigsuspend(const sigset_t *sigmask);
```

### 10.17 函数abort
将SIGBRT信号发送给调用进程，使其异常终止。POSIX声明abort不可忽略
```C
#include <stdlib.h>
void abort(void);
```

### 10.18 函数system
system调用者在等待命令完成时应忽略SIGINT和SIGQUIT，阻塞SIGCHLD

### 10.19 函数sleep
```C
#include <unistd.h>

unsigned int sleep(unsigned int seconds);
```
sleep调用会使进程挂起，直到：
1. 已经过了seconds的秒数， sleep返回值为0
2. 调用进程捕捉到一个信号，并从信号处理程序返回。此时sleep返回值为未休眠的秒数

### 10.20 函数sigqueue
信号队列
```C
#include <signal.h>
int sigqueue(pid_t pid, int signo, const union sigval value);
```

## 第十一章 线程

### 11.4 线程创建
```C
#include <pthread.h>

int pthread_create(pthread_t *restrict tidp,
    const pthread_attr_t *restrict attr,
    void *(*start_rtn)(void *), void *restrict arg
);
```
- 新线程ID会被设置成tidp指向的内存单元
- attr参数用于定制各种不同的线程属性
- 新线程从start_rtn函数的地址开始运行，arg指向函数的参数
- 线程创建时新线程和调用线程的运行顺序是不确定的
- 创建失败会返回错误码而非设置errno

### 11.5 线程终止
如果任意线程调用了exit、_Exit或_exit, 整个进程会终止

单个线程可以有三种方式退出
- 线程从启动线程中返回，返回值为线程的退出码
- 线程可以被同一进程中的其他线程取消
- 线程调用pthread_exit

退出进程，
```C
include <pthread.h>

void pthread_exit(void *rvla_ptr);
```

等待线程退出

```C
int pthread_join(pthread_t thread, void **rval_ptr);
```

取消线程

```C
int pthread_cancel(pthread_t tid);
```

清理

```C
#include <pthread.h>
void pthread_cleanup_push(void (*rtn) (void *), void *arg);
void pthread_cleanup_pop(int exucute);
```
- pthread_cleanup_push和pthread_cleanup_pop需要成对调用
- 清楚程序记录在栈中，执行顺序和注册顺序相反
- pthread_cleanup_push在MacOS上是用宏实现的，并把上下文存在了栈上，
  push和pop之间的执行过程中栈可能被改写

进程和线程之间相似的函数

|进程原语|线程原语|描述|
|---|---|---|
|fork|pthread_create|创建新的控制流|
|exit|pthread_exit|从现有控制流退出|
|waitpid|pthread_join|从控制流种得到退出状态|
|atexit|pthread_cancel_push|注册在退出控制流时的调用函数|
|getpid|pthread_self|h获取控制流ID|
|abort|pthread_cancel|请求控制流非正常退出|

### 11.6.1 互斥量

创建和销毁锁
```C
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *resitruct mutex, 
    const pthread_mutexatrr_t *resitrct attr);
    
int pthread_mutex_destory(pthread_mutex_t *mutex);
```

获取和释放锁

```C
#include <pthread.h>

init pthread_mutex_lock(pthread_mutex_t *mutex);
init pthread_mutex_trylock(pthread_mutex_t *mutex);
init pthread_mutex_unlock(pthread_mutex_t *mutex);
```
锁的粒度太粗，就会出现很多线程阻塞等待相同的锁，反之，过多的锁会开销会使性能受到影响，而且代码会变得复杂


限时获取锁
```C
#include <pthread.h>
#include <time.h>

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict tsptr);
```

### 11.6.4 读写锁

初始化和销毁
```C
#include <pthread.h>

int pthread_rwlock_init(pthread_rwlock_t *restric rwlock, const pthread_rwlockattr_t *restrict attr);
int pthread_rwlock_destory(pthread_rwlock_t *rwlock);
```

加解锁

```C
int pthead_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthead_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthead_rwlock_unlock(pthread_rwlock_t *rwlock);
```

### 11.6.6 条件变量
条件变量给多个线程提供了一个会和场所

### 11.6.7 自旋锁spin
自旋锁补充通过休眠是进程阻塞，而是在获取锁之前一直处于忙等阻塞状态，CPU一直被占据着
内核态中，自旋锁除了提供互斥机制外，他们会阻塞中断
通常适用于锁被持有时间短且不希望在重新调度上花费太多成本

### 11.6.8 屏障barrier
p屏障允许每个线程等待，直到多余的合作线程都到达某一点，然后从该点继续执行

```C
#include <pthread.h>

int pthread_barrier_init(pthread_barrier_t *restrict barrier, 
                           const pthread_barrierattr_t *restrict attr,
                           unsigned int count
);

int pthread_barrire_wait(pthread_barrier_t *barrier);

```

## 第十二章 线程控制

### 12.2 线程限制
- PTHREAD_KEYS_MAX    进程可以创建键(thread_local)的最大数目
- PTHREAD_STACK_MIN   线程栈最小字节数
- PTHREAD_THREADS_MAX 最大线程数

### 12.3 线程属性

```C
#include <pthread.h>

int pthread_atrr_init(pthread_attr_t *attr);

int pthread_atrr_destory(pthread_attr_t *attr);

int pthread_attr_getdetachstate(const pthread_attr_t *restrict attr, int *detachstate);

int pthread_attr_setdetachstate(const pthread_attr_t *restrict attr, int *detachstate);
```

设置线程stackarr

```C
#include <pthread.h>
int pthread_attr_getstacksize(const pthread_attr_t *resitrct attr, size_t *restrict stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

```

### 12.4 同步属性

#### 互斥量属性
- 进程共享属性
    - PTHREAD_PROCESS_PRIVATE 线程间共享
    - PTHREAD_PROCESS_SHARED  多进程共享
- 健壮属性
    - PTHREAD_MUTEX_STALLED
    - PTHREAD_MUTEX_ROBUST: 持有锁的进程在释放锁锁前终止了,另外的线程获取锁的时候会返回EOWNRDEAD而非0
- 类型属性
    - PTHREAD_MUTEX_NORMAL: 不做任何错误检查
    - PTHREAD_MUTEX_ERRORCHECK: 提供错误检查
    - PTHREAD_MUTEX_RECURSIVE: 同一线程可在锁释放前多次加锁

|类型|没解锁时重新加锁|不占用锁时解锁|已解锁时解锁|
|---|---|---|---|
|PTHREAD_MUTEX_NORMAL|死锁|未定义|未定义|
|PTHREAD_MUTEX_ERRORCHECK|返回错误|返回错误|返回错误|
|PTHREAD_MUTEX_RECURSIVE|允许|返回错误|返回错误|

### 12.5 重入
  - 线程安全：一个函数在相同的时间可以被多个线程安全调用
  - 异步信号安全： 函数的异步信号处理程序的重入是安全的
  
### 12.6 线程特定数据（ThreadLocal?）

```C
include <pthread.h>

int pthread_key_create(pthread_key_t *kep, void (*desctructor) (void*));

int pthread_key_delete(pthread_key_t key);

void *pthread_getspecific(pthread_key_t key);

int pthread_setspecific(ptrhead_key_t key, const void *value);
```

`pthread_key_create`创建key并关联析构函数，析构函数在线程正常退出时会被调用

`pthread_key_delete`取消key和线程特定数据的关联

`pthread_getspecific` 获取关联数据

`pthread_setspecific` 设置关联数据

### 12.7  取消选项

取消点：线程检查他是否被取消的一个位置

默认情况下线程在取消请求发出后还是继续运行，直到达到某个取消点

可以通过调用`pthread_testcancel`在程序中添加自己的取消点

取消类型有两种，默认为推迟取消，即遇到取消点才取消，另外一种是异步取消，可以在任意时间取消

### 12.8 线程和信号

信号的处理是进程中所有线程共享的

进程中的信号只会送到单个线程

可以通过`pthread_kill`给指定线程发信号


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
- [MeiK2333/apue](https://github.com/MeiK2333/apue)