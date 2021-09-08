#include <pty.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>

#include <ft_pty.h>
#include <ft_terminal.h>

ft_pty_t ft_pty;

int ft_pty_init(ft_pty_t *pty)
{
    int fdm =  posix_openpt(O_RDWR);
    if (fdm < 0) {
        printf("freeterm: opent ptm failed!\n");
        return -1;
    }
    unlockpt(fdm);
    grantpt(fdm);
    int flags = PTTY_RDNOBLK;
    ioctl(fdm, TIOCSFLGS, &flags);

    pty->fd_master = fdm;
    pty->initialized = 1;
    pty->pid_slaver = -1;
    return 0;
}

void exit_fn()
{
    kill(ft_pty.pid_slaver, SIGKILL);
    waitpid(ft_pty.pid_slaver, NULL, WUNTRACED);
}


int ft_pty_setup(pid_t pid)
{
    ft_pty.pid_slaver = pid; // 记录子进程的进程pid
    atexit(exit_fn);
    return 0;
}

int ft_pty_launch(ft_pty_t *pty, char *pathname)
{
    if (!pty->initialized)
        return -1;

    char *sname = ptsname(pty->fd_master);
    if (sname == NULL) {
        printf("freeterm: %s: get slaver name failed!\n", __func__);
        return -1;
    }
    #ifdef DEBUG_FT
    printf("freeterm: %s: slaver tty:%s\n", __func__, sname);
    #endif
    int fds = opendev(sname, O_RDWR);
    if (fds < 0) {
        printf("freeterm: %s: open slaver %s failed!\n", __func__, sname);
        return -1;
    }
    
    /* 为shell设置新组 */
    setpgrp();
    tcsetpgrp(fds, getpgrp());

    // 重定向 到stdin, stdout, stderr.
    dup2(fds, 0);
    dup2(fds, 1);
    dup2(fds, 2);
    close(fds);
    close(pty->fd_master);

    #ifdef DEBUG_FT
    printf("freeterm: %s: open slaver %s success.\n", __func__, sname);
    #endif
    execl(pathname, NULL);
    printf("freeterm: %s: execute shell %s failed!\n", __func__, pathname);
    exit(-1);
    return -1;
}

int ft_pty_exit(ft_pty_t *pty)
{
    if (!pty->initialized)
        return -1;
    
    close(pty->fd_master);
    return 0;
}
