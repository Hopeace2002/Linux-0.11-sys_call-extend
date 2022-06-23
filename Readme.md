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

git push origin master 推到github里
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

++++

添加系统调用号：

```c
// include/unistd.h

#define __NR_execve2 87
#define __NR_getdents 88
#define __NR_hopeace 89
#define __NR_sleep 90
#define __NR_getcwd 91

unsigned int sleep(unsigned int seconds);
int execve2(const char *path, char * argv[], char * envp[]);
int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
long getcwd(char * buf, size_t size);
int hopeace();
```



添加系统调用表

```C
// include/linux/sys.h
unsigned int sleep(unsigned int seconds);
int execve2(const char *path, char * argv[], char * envp[]);
int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
long getcwd(char * buf, size_t size);
int hopeace();

sys_call_table[] => 加上sys_execve2, sys_getdents, sys_hopeace, sys_sleep, sys_getcwd
    
```

添加函数具体实现（这里只有sys_sleep 实现）

```c
// kernel/sys.c

unsigned int sys_sleep(unsigned int seconds)
{
	sys_signal(14, SIG_IGN, NULL);
	sys_alarm(seconds);
	sys_pause();
	return 0;
}

int sys_execve2(const char *path, char * argv[], char * envp[])
{
	return 0;
}

int sys_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
	return 0;
}

long sys_getcwd(char * buf, size_t size)
{
	return 0;
}

int  sys_hopeace()
{
	return 0;
}
```

修改系统调用总数量

```c
// kernel/system_call.s
nr_system_calls = 92  /* 72 */
```

sleep实现

然后操作getcwd函数

删除kernel/sys.c中的sys_getcwd

并建立新文件

```c
// fs/dcatch.c
#include <string.h>
#include <linux/mm.h>
#include <linux/fs.h>


long sys_getcwd(char * buf, size_t size)
{
	return 0;
}
```

为v0.1.0版本

新建/fs/dcatch.c文件

```c
#include <string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <../mm/memory.c>

#define __GFP_WAIT	0x01
#define __GFP_IO	0x02
#define __GFP_LOW	0x00
#define GFP_USER	(__GFP_LOW | __GFP_WAIT | __GFP_IO)
// long sys_getcwd(char * buf, size_t size)
// {
//     int error;
//     unsigned long len;
//     char * page = (char *)get_free_page(GFP_USER);
//     char * pwd 
// 	return 0;
// }

long sys_getcwd(char * buf, size_t size)
{
    unsigned short lst_inode;
    struct m_inode *new_inode;
    char* pathname[128]; // range;
    char *ans;
    struct m_inode *inode = current->pwd; // 当前目录索引节点
    struct buffer_head *path_head = bread(current->root->i_dev, inode->i_zone[0]);
    struct dir_entry *getpath = (struct dir_entry *)path_head->b_data; // 第一个目录项
    int i = 0;
    while(1) {
        lst_inode = getpath->inode;
        new_inode = iget(current->root->i_dev, (getpath + 1)->inode);
        path_head = bread(current->root->i_dev, new_inode->i_zone[0]);
        getpath = (struct dir_entry*) path_head->b_data;
        int j = 1;
        while(1) {
            if ((getpath + j)->inode == lst_inode)
            {
                break;
            }
            j++;
        }
        if (j==1)
        {
            break;
        }
        pathname[i] = (getpath + j)->name;
        i++;
    }
    int count = 0;
    i--;
    while (i >= 0)
    {
        int k = 0;
        ans[count++] = '/';
        while(pathname[i][k] != '\0')
        {
            ans[count] = pathname[i][k];
            k++;
            count++;
        }
        i--;
    }
    for (k = 0; k < count; k++)
    {
        put_fs_byte(ans[k], buf+k);
    }
    return (long)(ans);
}
```

修改fs/Makefile

```makefile
OBJS=	open.o read_write.o inode.o file_table.o buffer.o super.o \
	block_dev.o char_dev.o file_dev.o stat.o exec.o pipe.o namei.o \
	bitmap.o fcntl.o ioctl.o truncate.o select.o dcatch.o
	
### Dependencies:
dcatch.o: dcatch.c ../include/string.h ../include/linux/fs.h\
  ../include/linux/mm.h ../fs/memory.c
```

报错

 *** 没有规则可以创建“dcatch.o”需要的目标“../fs/memory.c”。

尝试修改为./memory.c

还是不可以

修改引用C文件为\#include "../mm/memory.c"

解决办法：把依赖里的memory.c再删掉就ok了

fs/fs.o：在函数‘sys_getcwd’中：
(.text+0x8245)：对‘put_fs_byte’未定义的引用

明天再解决吧

dcatch 添加引用

\#include <asm/segment.h>

事实证明

\#include "../mm/memory.c"会出现重复定义的问题

去找那些定义在.h中的带有extern的函数

```C
#include <string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include "linux/sched.h"
```

成功编译，实现了getcwd！



**接下来去操作getdents函数**

结果意外发现已经通过了测试？？？

难道是检验程序出bug了？

明天再测吧，要睡觉了



新的一天，让我继续进行愉快的实验吧！

今天的任务是实现getdents函数以及了解execve2的工作原理

新建文件fs/readdir.c

把基本的东西引入一下

```C
#include <include/sys/types.h>

// 引入结构体
struct linux_dirent
{
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[];
};


int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
```

查看linux 1.3.0内核中的sys_getdents代码

```c
int sys_getdents(unsigned int fd, void * dirent, unsigned int count)
{
	struct file * file;
	struct linux_dirent * lastdirent;
	struct getdents_callback buf;
	int error;

	if (fd >= NR_OPEN || !(file = current->files->fd[fd]))
		return -EBADF;
	if (!file->f_op || !file->f_op->readdir)
		return -ENOTDIR;
	error = verify_area(VERIFY_WRITE, dirent, count);
	if (error)
		return error;
	buf.current = (struct linux_dirent *) dirent;
	buf.previous = NULL;
	buf.count = count;
	buf.error = 0;
	error = file->f_op->readdir(file->f_inode, file, &buf, filldir);
	if (error < 0)
		return error;
	lastdirent = buf.previous;
	if (!lastdirent)
		return buf.error;
	put_user(file->f_pos, &lastdirent->d_off);
	return count - buf.count;
}
```



```c
// fs/readdir.c
int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
    struct linux_dirent lastdirent;
    struct m_inode *inode = current->filp[fd]->f_inode;
    struct buffer_head *path_head = bread(inode->i_dev, inode->i_zone[0]);
    struct dir_entry *path = (struct dir_entry *)path_head->b_data;
    int temp = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (path->inode == 0 || (i+1) * 24 > count)
            break;
            lastdirent.d_ino = path[i].inode;
            for (int j = 0; j < 14; j++)
            {
                lastdirent.d_name[j] = path[i].name[j];
            }
            lastdirent.d_off = 0;
            lastdirent.d_reclen = 24;
            for (int k = 0; k < 24; k++){
                put_fs_byte(((char *)&lastdirent)[k], ((char *)dirp + temp));
                temp++;
            }
    }
    return temp;
}
```



```c
// fs/Makefile
OBJS => 添加readdir.o

### Dependencies:
readdir.o: readdir.c ../include/linux/fs.h ../include/asm/segment.h\
  ../include/linux/sched.h  ../include/sys/types.h
```

编译报错

readdir.c: In function ‘getdents’:
readdir.c:22:5: error: ‘for’ loop initial declarations are only allowed in C99 mode
     for (int i = 0; i < 1024; i++)
     ^
readdir.c:22:5: note: use option -std=c99 or -std=gnu99 to compile your code

难道是要在外边去声明i、j、k？

改完之后，通过编译

再次测试发现出大问题

getcwd返回的目录竟然是/usr/t（实际上是/usr/root/1）

再次检查一下dcatch.c 文件吧

是不是数组开小了

```
char* pathname[128]; // range;
=> char* pathname[512];
```

还是不可以

检查发现是k的定义出现在了循环的外面

修改为

```c
int k;
    while (i >= 0)
    {
        k = 0;
        ans[count++] = '/';
        while(pathname[i][k] != '\0')
        {
            ans[count] = pathname[i][k];
            k++;
            count++;
        }
        i--;
    }
```

这样测试后就得到正确的结果啦

/usr/root/1

啊啊啊

getdents忘记了

应该定义为sys_getdents的

同时删掉之前sys.c中定义的sys_getdents，避免重复定义

再次编译测试发现回显了"."open fd:3 getdents fd: 48

起码不是空的了

找到问题所在了,结构体中的d_name不是空

```c
struct linux_dirent
{
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[14];
};
```

