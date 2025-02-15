/**
 * Test program to open the serial connection via USB to the RockBLOCK 9603
 * and interact with it to be ran on a Mac.
 *
 * Author: Jordan Bourdeau
 */

#include <chrono>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <poll.h>
#include <stdint.h>
#include <sstream>
#include <stdio.h>
#include <termios.h>
#include <thread>
#include <unistd.h>

const int MAX_RESPONSE_LENGTH = 256;
static char RESPONSE_BUFFER[MAX_RESPONSE_LENGTH];
const int TIMEZONE_OFFSET = -60 * 60 * 4;

class SerialPort {
public:
  SerialPort(const char *portName) {
    fd = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
      perror("Unable to open serial port");
    } else {
      fcntl(fd, F_SETFL, 0); // Clear the non-blocking flag
      configure_port();
    }
  }

  ~SerialPort() {
    if (fd != -1) {
      close(fd);
    }
  }

  /**
   * Configure the terminal to operate in raw mode for sending and receiving.
   */
  void configure_port() {
    struct termios options;
    tcgetattr(fd, &options);
    cfsetspeed(&options, B19200);
    options.c_cflag = (options.c_cflag & ~CSIZE) | CS8; // 8 data bits
    options.c_cflag &= ~PARENB;                         // No parity
    options.c_cflag &= ~CSTOPB;                         // 1 stop bit
    options.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore control lines

    options.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    options.c_lflag &=
        ~(ECHO | ECHOE | ECHONL | ICANON | ISIG | IEXTEN); // Raw input
    options.c_oflag &= ~OPOST;                             // Raw output

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
      perror("Error setting serial port attributes");
      exit(1);
    }

    // Flush buffers
    tcflush(fd, TCIFLUSH);
  }

  int get_time(std::tm &time) {
    static char get_time[] = "AT-MSSTM";
    static char sub[] = "MSSTM: ";
    static std::tm iridium_epoch = {
        55,          // tm_sec
        23,          // tm_min
        14,          // tm_hour
        11,          // tm_mday
        4,           // tm_mon (0 = January, so 4 = May)
        2014 - 1900, // tm_year (years since 1900)
        0,           // tm_wday (not set)
        0,           // tm_yday (not set)
        -1           // tm_isdst (let the library determine DST)
    };

    if (int rc = send_receive(get_time, RESPONSE_BUFFER)) {
      return rc;
    }
    // Expect a response in the form of 0xXXXXXXXX which is a 32-bit hex
    // ascii character string representing the number of 90 millisecond
    // intervals that have elapsed since the Iridium epoch.
    char *start = strstr(RESPONSE_BUFFER, sub) + strlen(sub);
    if (!start) {
      fprintf(stderr, "Did not find time. Instead got response:\n%s",
              RESPONSE_BUFFER);
      return 1;
    }

    uint64_t intervals = std::stoull(start, nullptr, 16);

    // Calculate total seconds elapsed since the Iridium epoch
    uint64_t total_ms = intervals * 90; // 90 milliseconds per interval
    uint64_t total_seconds = total_ms / 1000;

    // Convert to time_t for easier manipulation
    std::time_t epoch_time = std::mktime(&iridium_epoch);
    epoch_time += total_seconds;
    epoch_time += TIMEZONE_OFFSET;

    // Convert to local time (EST)
    std::tm *local_time = std::localtime(&epoch_time);
    time = *local_time;

    return 0; // Indicate success
  }

  /**
   * Send a command, wait for a response, then write it to the destination
   * buffer and print it to the screen.
   *
   * @returns (int): Return code.
   */
  int send_receive(const char *command, char dst[MAX_RESPONSE_LENGTH]) {
    fprintf(stdout, "Sending \"%s\"\n", command);
    int cmd_len = strlen(command);
    int n_written = write(fd, command, cmd_len);

    // Write a carriage return to signal end of command and wait for response.
    write(fd, "\r", 1);
    fprintf(stdout, "Wrote %d bytes\n", n_written);

    // Wait until the first byte becomes available, then wait some time to
    // allow the full response to be received
    int rc;
    struct pollfd pfd;
    pfd.fd = fd;       // The file descriptor to monitor
    pfd.events = POLLIN; // We're interested in reading
    pfd.revents = 0;

    while (true) {
        rc = poll(&pfd, 1, 1000);  // Poll with a 1-second timeout

        if (rc > 0) {
            if (pfd.revents & POLLIN) {
                // Slight delay to get all the response
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
            }
        } else if (rc == 0) {
            printf("Timeout occurred, no data yet\n");
        } else {
            perror("poll failed");
            break;
        }
    }

    int n_read = read(fd, dst, MAX_RESPONSE_LENGTH - 1);
    if (n_read > 0) {
      dst[n_read] = '\0'; // Null terminate the string
      fprintf(stdout, "Received %d bytes: %s\n", n_read, dst);
    } else {
      fprintf(stderr, "Error reading from the serial port.\n");
      return 1;
    }
    return 0;
  }

private:
  int fd;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "USAGE: main \"<cmd>\"\n");
    return 1;
  }
  // Hardcoded for now
  const char *portName = "/dev/ttyUSB0";
  SerialPort serial(portName);

  if (strcmp(argv[1], "time") == 0) {
    std::tm time;
    int rc = serial.get_time(time);
    if (rc) {
      return rc;
    }

    // Print the time in EST
    std::cout << "Timestamp (EST): "
              << std::put_time(&time, "%Y-%m-%d %H:%M:%S") << " EST"
              << std::endl;
    return 0;
  } else {
    return serial.send_receive(argv[1], RESPONSE_BUFFER);
  }
}
