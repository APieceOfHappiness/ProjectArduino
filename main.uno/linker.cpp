#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <unistd.h>

#define DURATION 100
#define PRINTER_PATH "../pyscript/main.py"
#define DATA_PATH "../pyscript/data.txt"
#define LOCK_PATH "../pyscript/lock.txt"


struct SensorData {
    double preasure_val;
    double temperature_val;
    double sound_val;
    double light_val;
};

void calc_mean(struct SensorData* mean, struct SensorData* sum, size_t cnt) {
    mean->preasure_val = sum->preasure_val / cnt;
    mean->temperature_val = sum->temperature_val / cnt;
    mean->sound_val = sum->sound_val / cnt;
    mean->light_val = sum->light_val / cnt;  
}

void write_sensor_data_to_pipe(int named_pipe, struct SensorData *sensor_data_mean) {
    write(named_pipe, &sensor_data_mean->preasure_val, sizeof(sensor_data_mean->preasure_val));
    write(named_pipe, &sensor_data_mean->temperature_val, sizeof(sensor_data_mean->temperature_val));
    write(named_pipe, &sensor_data_mean->sound_val, sizeof(sensor_data_mean->sound_val));
    write(named_pipe, &sensor_data_mean->light_val, sizeof(sensor_data_mean->light_val));
}

void sub_from_sum(struct SensorData *dest, struct SensorData *src) {
    dest->light_val -= src->light_val;
    dest->preasure_val -= src->preasure_val;
    dest->sound_val -= src->sound_val;
    dest->temperature_val -= src->temperature_val;
}

void add_to_sum(struct SensorData *dest, struct SensorData *src) {
    dest->light_val += src->light_val;
    dest->preasure_val += src->preasure_val;
    dest->sound_val += src->sound_val;
    dest->temperature_val += src->temperature_val;
}

void get_sensor_data(FILE *port, struct SensorData *sensor_data) {
    if (fscanf(port, 
           " %lf %lf %lf %lf ", 
           &sensor_data->preasure_val,
           &sensor_data->temperature_val,
           &sensor_data->sound_val,
           &sensor_data->light_val) != 4) {
        perror("fscanf");
    };
}

int open_port() {
  int fd; /* Файловый дескриптор для порта */

  fd = open("/dev/ttyACM0", O_RDWR  | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
    perror("open_port: Unable to open /dev/ttyS0 - ");
    _exit(-EXIT_FAILURE);
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
    printf("Ok - Port open!\n");
  }

  return fd;
}

int main() {
    int num;
    int64_t sum_values = 0;

    sleep(2);  // Calibration
    int port_fd = open_port(); // Open serial port
    FILE* fport = fdopen(port_fd, "r");
    if (!fport) {
        perror("fdopen");
        _exit(-EXIT_FAILURE);
    }

    std::queue<struct SensorData> ma_sensor_data; // MA - Moving Average
    struct SensorData sensor_data_cur;
    struct SensorData sensor_data_sum;
    struct SensorData sensor_data_mean;

    for (size_t i = 0; i < DURATION; ++i) { // Start initializing
        get_sensor_data(fport, &sensor_data_cur);
        add_to_sum(&sensor_data_sum, &sensor_data_cur);
        ma_sensor_data.push(sensor_data_cur);
    }

    int python_printer = fork(); // Open UI with plots
    if (python_printer < 0) {
        _exit(-EXIT_FAILURE);
    } else if (python_printer == 0) {
        execlp("python3", "python3", PRINTER_PATH, "x", "y", DATA_PATH, LOCK_PATH, NULL);
        printf("python3 crashed\n");
        return -EXIT_FAILURE;
    }

    if (!mkfifo("/tmp/my_pipe", 0666)) {
        perror("mkfifo");
        _exit(-EXIT_FAILURE);
    };
    int named_pipe = open("/tmp/my_pipe", O_WRONLY);
    if (!named_pipe) {
        perror("fopen");
        _exit(-EXIT_FAILURE);    
    }

    FILE* bd = fopen("../pyscript/data.txt", "a");
    if (!bd) {
        perror("fopen");
        _exit(-EXIT_FAILURE);
    }

    while (1) {

        calc_mean(&sensor_data_mean, &sensor_data_sum, DURATION);  

        write_sensor_data_to_pipe(named_pipe, &sensor_data_mean);

        get_sensor_data(fport, &sensor_data_cur);
        ma_sensor_data.push(sensor_data_cur);
        add_to_sum(&sensor_data_sum, &sensor_data_cur);

        sub_from_sum(&sensor_data_sum, &ma_sensor_data.front());
        ma_sensor_data.pop();
    }

    fclose(fport);
    if (wait(NULL) < 0) {
        _exit(-EXIT_FAILURE);
    };
    return EXIT_SUCCESS;
}