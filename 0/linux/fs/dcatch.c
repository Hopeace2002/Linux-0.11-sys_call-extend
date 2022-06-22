#include <string.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include "../mm/memory.c"

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
    int k = 0;
    while (i >= 0)
    {
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