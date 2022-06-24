#include <sys/types.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/segment.h>
// 引入结构体
struct linux_dirent
{
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[14];
};
/*
 *  sys_getdents 获取当前目录所有文件名
 *  成功执行返回读取的字节数
 *  到当前目录结尾返回0
 *  失败返回-1
 */
int sys_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
    // 类似于getcwd, 先获得逻辑块
    struct linux_dirent lastdirent;
    // 得到索引节点inode
    struct m_inode *inode = current->filp[fd]->f_inode;
    // dev，block
    struct buffer_head *path_head = bread(inode->i_dev, inode->i_zone[0]);
    // 第一个目录项
    struct dir_entry *path = (struct dir_entry *)path_head->b_data;
    int bytes_num = 0;
    int i, j, k;
    for (i = 0; i < 1024; i++)
    {
        // 缓冲区溢出
        if (path->inode == 0 || (i+1) * 24 > count){
            break;
        }
        lastdirent.d_ino = path[i].inode;
        for (j = 0; j < 14; j++)
        {
            lastdirent.d_name[j] = path[i].name[j];
        }
        lastdirent.d_off = 0;
        lastdirent.d_reclen = 24; 
        for (k = 0; k < 24; k++){
            // put_fs_byte同getcwd中的使用
            put_fs_byte(((char*)&lastdirent)[k], ((char*)dirp+bytes_num));
            bytes_num++;
        }
    }
    // 到结尾返回0
    if (bytes_num == 24)
    {
        return 0;
    }
    return bytes_num;
}
