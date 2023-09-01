#include "Mem.h"


int pidA=0;

bool pvm(void *address, void *buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (pidA < 0) {
        return false;
    }

#if defined(__arm__)
    int process_vm_readv_syscall = 376;
    int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
    int process_vm_readv_syscall = 270;
    int process_vm_writev_syscall = 271;
#elif defined(__i386__)
    int process_vm_readv_syscall = 347;
    int process_vm_writev_syscall = 348;
#else
    int process_vm_readv_syscall = 310;
    int process_vm_writev_syscall = 311;
#endif

    ssize_t bytes = syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall),
                            pidA, local, 1, remote, 1, 0);
    //printf("process_vm_readv reads %zd bytes from PID: %d\n", bytes, pidA);
    return bytes == size;
}

bool vm_readv(void *address, void *buffer, size_t size) {
    return pvm(address, buffer, size, false);
}


pid_t find_pid(const char *process_name) {
    int id;
    pid_t pid = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];

    struct dirent *entry;
    if (process_name == NULL) {
        return -1;
    }
    dir = opendir("/proc");
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);

                if (strcmp(process_name, cmdline) == 0) {
                    /* process found */
                    pid = id;
                    break;
                }
            }
        }
    }

    closedir(dir);
    pidA = pid;
    return pid;
}

kaddr get_module_base(const char *module_name) {
    FILE *fp;
    kaddr addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pidA);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}
