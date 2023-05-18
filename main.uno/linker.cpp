#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define DURATION 1
#define PRINTER_PATH "../pyscript/main.py"
#define DATA_PATH "../pyscript/data.txt"
#define LOCK_PATH "../pyscript/lock.txt"

int open_port() {
  int fd; /* Файловый дескриптор для порта */

  fd = open("/dev/ttyACM0", O_RDWR  | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
    perror("open_port: Unable to open /dev/ttyS0 - ");
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
    printf("Ok - Port open!\n");
  }

  return fd;
}

int main() {
    
    int python_printer = fork();
    if (python_printer < 0) {
        _exit(-EXIT_FAILURE);
    } else if (python_printer == 0) {
        execlp("python3", "python3", PRINTER_PATH, "x", "y", DATA_PATH, LOCK_PATH, NULL);
        printf("python3 crashed\n");
        return -EXIT_FAILURE;
    }

    sleep(2);  // Calibration
    int port_fd = open_port();
    int num;
    FILE* fport = fdopen(port_fd, "r");
    int64_t sum_values = 0;
    for (int i = 1;; i = (i + 1) % (DURATION + 1)) {
        if (i == 0) {
            FILE* flock = fopen(LOCK_PATH, "r");
            if (!flock) {
                perror("flock");
                _exit(-EXIT_FAILURE);
            }  

            char flag;
            int res = 0;
            if ((res = fscanf(flock, "%c", &flag)) != 1) {
                printf("fscanf %d\n", res);
                sum_values = 0;
                continue;
                // _exit(-EXIT_FAILURE);
            }
            fclose(flock);

            if (flag == 'r') {
                sum_values = 0;
                continue;
            } else if (flag == 'w') {
                FILE *fdata = fopen(DATA_PATH, "w");
                printf("%ld\n", sum_values);
                fprintf(fdata, "%ld\n", sum_values / (DURATION + 1));

                flock = fopen(LOCK_PATH, "w");
                if (!flock) {
                    perror("flock");
                    _exit(-EXIT_FAILURE);
                }                
                fprintf(flock, "r");
                fclose(fdata);
            }
            fclose(flock);
            sum_values = 0;
        } else {
            int res;
            if ((res = fscanf(fport, "%d", &num)) <= 0) {
                printf("WHAT??");
            };
            sum_values += num;
            printf("num %d %d\n", num, res);
        }
    }
    fclose(fport);
    if (wait(NULL) < 0) {
        _exit(-EXIT_FAILURE);
    };
    return EXIT_SUCCESS;
}