#ifndef _LIB_UNISTD_H
#define _LIB_UNISTD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>
#include <stddef.h>
#include <fcntl.h>

/* 高4位是属性位 */
#define S_IFSOCK 0x90    //scoket
#define S_IFLNK 0x50     //符号连接
#define S_IFIFO 0x30     //先进先出
#define S_IFBLK 0x80     //区块装置
#define S_IFCHR 0x40     //字符装置
#define S_IFDIR 0x20     //目录
#define S_IFREG 0x10     //一般文件

#define S_IREAD 0x04     //文件所有者具可读取权限
#define S_IWRITE 0x02    //文件所有者具可写入权限
#define S_IEXEC 0x01     //文件所有者具可执行权限

//上述的文件类型在POSIX中定义了检查这些类型的宏定义：


#define S_ISDIR(m)			((m) & S_IFDIR )    //是否为目录
#define S_ISCHR(m)			((m) & S_IFCHR )    //是否为字符设备
#define S_ISBLK(m)			((m) & S_IFBLK )    //是否为块设备
#define S_ISREG(m)			((m) & S_IFREG )    //是否为一般文件
#define S_ISLNK(m)			((m) & S_IFLNK )    //判断是否为符号连接
#define S_ISFIFO(m)			((m) & S_IFIFO )    //先进先出
#define S_ISSOCK(m)			((m) & S_IFSOCK )   //是否为socket

/* 文件模式 */
#define M_IREAD  (1 << 2)     //文件可读取权限
#define M_IWRITE (1 << 1)    //文件可写入权限
#define M_IEXEC  (1 << 0)     //文件可执行权限

#ifndef SEEK_SET
/* file seek */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

/* file acesss 文件检测 */
#define R_OK 4 /* Test for read permission. */
#define W_OK 2 /* Test for write permission. */
#define X_OK 1 /* Test for execute permission. */
#define F_OK 0 /* Test for existence. */

#define STDIN_FILENO    0  /* 标准输入文件号 */
#define STDOUT_FILENO   1  /* 标准输出文件号 */
#define STDERR_FILENO   2  /* 标准错误文件号 */

int brk(void *addr);
void *sbrk(int increment);

int open(const char *path, int flags, ...);
int close(int fd);
int read(int fd, void *buffer, size_t nbytes);
int write(int fd, const void *buffer, size_t nbytes);
int lseek(int fd, off_t offset, int whence);
int access(const char *filenpath, int mode);
int unlink(const char *path);
int ftruncate(int fd, off_t offset);
int fsync(int fd);
int ioctl(int fd, int cmd, void *arg);
int fcntl(int fd, int cmd, long arg);

int dup(int fd);
int dup2(int oldfd, int newfd);

long tell(int fd);

int mkdir(const char *path, mode_t mode);
int rmdir(const char *path);
int _rename(const char *source, const char *target);

int chdir(const char *path);
int getcwd(char *buf, int bufsz);

int execve(const char *pathname, char *const argv[], char *const envp[]);
int execle(const char *pathname, char *const envp[], const char *arg, ...);
int execv(const char *pathname, char *const argv[]);
int execl(const char *pathname, const char *arg, ...);
int execvp(const char *filename, char *const argv[]);
int execlp(const char *filename, const char *arg, ...);
int usleep(useconds_t usec);

extern char **_environ;
#define environ _environ

int pipe(int fd[2]);

/* supported by xbook kernel */
int probedev(const char *name, char *buf, size_t buflen);
int opendev(const char *devname, int flags);
int openfifo(const char *fifoname, int flags);

#include <sys/proc.h>
#include <getopt.h>

#ifdef __cplusplus
}
#endif

#endif  /* _LIB_UNISTD_H */
