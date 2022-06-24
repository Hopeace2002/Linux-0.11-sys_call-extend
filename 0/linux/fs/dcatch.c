#include <string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/sched.h>


// #define __GFP_WAIT	0x01
// #define __GFP_IO	0x02
// #define __GFP_LOW	0x00
// #define GFP_USER	(__GFP_LOW | __GFP_WAIT | __GFP_IO)


/*
 *  sys_getcwd 用于实现linux pwd命令
 *  成功，返回当前工作目录的字符串的指针。
 *  失败，返回NULL
 */

long sys_getcwd(char * buf, size_t size)
{
    struct m_inode *inode = current->pwd; // current->pwd当前目录索引节点
    // bread(dev, block) 返回一个指向缓冲头的指针 buffer_head
    struct buffer_head *path_head = bread(current->root->i_dev, inode->i_zone[0]);
    // buffer_head 中b_data即对应的第一个逻辑块
    struct dir_entry *getpath = (struct dir_entry *)path_head->b_data; // 第一个目录项
    
    unsigned short lst_inode;
    struct m_inode *new_inode;
    char* pathname[512]; // range;
    char *ans;
    
    int i = 0;
    while(1) {
        // 记录当前节点号
        lst_inode = getpath->inode;
        // getpath + 1 表示../即上级目录
        new_inode = iget(current->root->i_dev, (getpath + 1)->inode);
        path_head = bread(current->root->i_dev, new_inode->i_zone[0]);
        getpath = (struct dir_entry*) path_head->b_data;

        // 在上级目录中遍历哪个为当前子目录
        int j = 1;
        while(1) {
            if ((getpath + j)->inode == lst_inode)
            {
                break;
            }
            j++;
        }
        // j == 1即已经是根目录(第一个是. 第二个是.. 但在根目录下，没有父目录了)
        if (j == 1)
        {
            break;
        }
        pathname[i] = (getpath + j)->name;
        i++;
    }

    // 组装pathname，最后添上跟目录标记'/'
    int count = 0;
    i--;
    int k;
    if (i < 0)
    {
        return NULL;
    }
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
    if (count < 0)
    {
        return NULL;
    }
    // put_fs_byte向用户空间中addr地址处写一个字节的内容
    for (k = 0; k < count; k++)
    {
        put_fs_byte(ans[k], buf+k);
    }
    return (long)(ans);
}