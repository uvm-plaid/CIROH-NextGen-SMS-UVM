/**
 * Test program to open the serial connection via USB to the RockBLOCK 9603
 * and interact with it to be ran on a Mac.
 *
 * Author: Jordan Bourdeau
 */

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

  /**
   * Send a command, wait for a response, then print it to the screen.
   */
  int send_receive(const char *command) {
    static char buffer[256];
    fprintf(stdout, "Sending \"%s\"\n", command);
    int cmd_len = strlen(command);
    int n_written = write(fd, command, cmd_len);

    // Write a carriage return to signal end of command and wait for response.
    write(fd, "\r", 1);
    sleep(1);

    fprintf(stdout, "Wrote %d bytes\n", n_written);

    int n_read = read(fd, buffer, sizeof(buffer) - 1);
    if (n_read > 0) {
      buffer[n_read] = '\0'; // Null terminate the string
      fprintf(stdout, "Received %d bytes: %s\n", n_read, buffer);
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
  const char *portName = "/dev/tty.usbserial-FTCZUOWG";
  SerialPort serial(portName);
  return serial.send_receive(argv[1]);
}
