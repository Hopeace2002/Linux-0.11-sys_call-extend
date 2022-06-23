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
    char d_name[];
};

int getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count)
{
    struct linux_dirent lastdirent;
    struct m_inode *inode = current->filp[fd]->f_inode;
    struct buffer_head *path_head = bread(inode->i_dev, inode->i_zone[0]);
    struct dir_entry *path = (struct dir_entry *)path_head->b_data;
    int temp = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    for (i; i < 1024; i++)
    {
        if (path->inode == 0 || (i+1) * 24 > count){
            break;
        }
        lastdirent.d_ino = path[i].inode;
        for (j; j < 14; j++)
        {
            lastdirent.d_name[j] = path[i].name[j];
        }
        lastdirent.d_off = 0;
        lastdirent.d_reclen = 24;
        for (k; k < 24; k++){
            put_fs_byte(((char*)&lastdirent)[k], ((char*)dirp+temp));
            temp++;
        }
    }
    return temp;
}
