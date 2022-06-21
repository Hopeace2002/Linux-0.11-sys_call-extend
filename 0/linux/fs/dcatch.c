#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/include/errno.h>
#include <linux/include/linux/sched.h>
#define __GFP_WAIT	0x01
#define __GFP_IO	0x02
#define __GFP_LOW	0x00
#define GFP_USER	(__GFP_LOW | __GFP_WAIT | __GFP_IO)
#define current (get_current())
/*
 * "buflen" should be PAGE_SIZE or more.
 */
char * d_path(struct dentry *dentry, char *buffer, int buflen)
{
	char * end = buffer+buflen;
	char * retval;
	struct dentry * root = current->fs->root;

	*--end = '\0';
	buflen--;
	if (dentry->d_parent != dentry && list_empty(&dentry->d_hash)) {
		buflen -= 10;
		end -= 10;
		memcpy(end, " (deleted)", 10);
	}

	/* Get '/' right */
	retval = end-1;
	*retval = '/';

	for (;;) {
		struct dentry * parent;
		int namelen;

		if (dentry == root)
			break;
		dentry = dentry->d_covers;
		parent = dentry->d_parent;
		if (dentry == parent)
			break;
		namelen = dentry->d_name.len;
		buflen -= namelen + 1;
		if (buflen < 0)
			break;
		end -= namelen;
		memcpy(end, dentry->d_name.name, namelen);
		*--end = '/';
		retval = end;
		dentry = parent;
	}
	return retval;
}
struct task_struct *get_current(void)
{
	struct task_struct *ts;
	__asm__ __volatile__("
	bic	%0, sp, #0x1f00
	bic	%0, %0, #0x00ff
	" : "=r" (ts));
	return ts;
}

long getcwd(char * buf, size_t size)
{
    int error;
    unsigned long len;
    char * page = (char *)get_free_page(GFP_USER);
    char * cwd = d_path(current->fs->pwd, page, PAGE_SIZE);
    error = -ERANGE;
    len = PAGE_SIZE + page - cwd;
    if (len <= size) {
        error = len;
        if (copy_to_user(buf, cwd, len))
            error = -EFAULT;
    }
    return error;
}