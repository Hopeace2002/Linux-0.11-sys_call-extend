[TOC]

## 操作系统一些系统调用功能的扩展

### 0x01 前言：

Git已经可以正常使用了

这里记录一些常用的命令，或者说是基本命令

```shell
// 初始化创建一个git仓库
git init

// 把文件添加到仓库
git add x.file

// 提交到版本库
git commit -m "ZhuShi"

// 查看文件状态
git status

// 查看修改记录
git log --pretty=oneline

// 回退版本，上个版本是HEAD^,上上个是HEAD^^,100个之前是HEAD~100
// 也可以输入版本id号具体回退，注意倒退后再想回来就只能通过版本号了
git reset --hard HEAD^
git reset --hard 1e5d(id号)
```

那么试着记录现在的状态到git里吧，然后我们就可以正式开始了

版本记录号采用0.0.0这样

修改某个文件具体代码变动第三位

修改一组文件实现某个具体功能变动第二位

修改实现新的一些功能变动第一位

开始吧！



### 0x02 分析拓展系统调用实验要求

参考博客：

[Linux内核学习系列（7）——execve与需求加载](https://blog.csdn.net/m0_37637511/article/details/123905117?ops_request_misc=%7B%22request%5Fid%22%3A%22165551663516780357251734%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=165551663516780357251734&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-1-123905117-null-null.142^v17^pc_search_result_control_group,157^v15^new_3&utm_term=execve延迟加载&spm=1018.2226.3001.4187)

[通过do_execve源码分析程序的执行（上）（基于linux0.11)](https://cloud.tencent.com/developer/article/1507765)

[GETDENTS - Linux手册页)](https://www.onitroad.com/jc/linux/man-pages/linux/man2/getdents.2.html)

[sleep - Linux manual page)](https://www.man7.org/linux/man-pages/man3/sleep.3.html)

[getcwd - Linux manual page ](https://www.man7.org/linux/man-pages/man3/getcwd.3.html)

以版本0内核为基础，增加一组系统调用，并通过给定的测试用例

- execve2
- getdents
- sleep
- getcwd

```
# execve2
以“立即加载”的方式执行一个可执行文件，要求加载完后运行时该进程不产生缺页异常
重点在于do_execve函数的修改
```



```c
// getdents获取目录条目，可以理解为读取各个文件名去展示
int getdents(unsigned int fd, struct linux_dirent *dirp,
             unsigned int count);
// 系统调用getdents()从打开文件描述符fd所引用的目录中读取几个linux_dirent结构到dirp所指向的缓冲区中。参数count指定该缓冲区的大小

struct linux_dirent {
    unsigned long  d_ino;     /* Inode number 索引节点号*/
    unsigned long  d_off;     /* Offset to next linux_dirent 
    							 目录到下一个linux_dirent的距离*/
    unsigned short d_reclen;  /* Length of this linux_dirent 
								 整个linux_dirent的大小*/
    char           d_name[];  /* Filename (null-terminated) */
                      /* length is actually (d_reclen - 2 -
                         offsetof(struct linux_dirent, d_name))
                         以空值结尾的文件名*/
    /*
    char           pad;       // Zero padding byte
    char           d_type;    // File type (only since Linux
                              // 2.6.4); offset is (d_reclen - 1)
                              // 结构末尾的字节，指示文件类型
                              // linux2.6.4之前访问此字段始终提供0(DT_UNKNOWN)
    */
}
```



```c
// sleep - sleep for a specified number of seconds
#include <unistd.h>
unsigned int sleep(unsigned int seconds);
// 返回值 - Zero if the requested time has elapsed, or the number of seconds left to sleep, if the call was interrupted by a signal handler.
// 使进程进入睡眠多久
```



```c
// getcwd
#include <unistd.h>
char *getcwd(char *buf, size_t size);
// 用来返回当前目录，类似于linux-pwd命令
```



#### execve2( )

加载可执行文件有两种：编译好的二进制文件和shell脚本文件



#### getdents( )



#### sleep( )

```
// kernal/sys.c
unsigned int sys_sleep(unsigned int seconds)
{
	sys_signal(14, SIG_IGN, NULL);
	sys_alarm(seconds);
	sys_pause();
	return 0;
}
```

14 是SIGALARM

SIG_IGN表示忽略

#### getcwd( )



### 0x03 正式操作







