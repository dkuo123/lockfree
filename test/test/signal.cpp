#include <errno.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>

volatile bool g_got_signal = false;

void handler(int signal) {
   g_got_signal = true;
}

void retry_write(int fd, char *buf, size_t len) {
   while (true) {
      ssize_t res = write(fd, buf, len);
      if (res == len) return;
      else if (res == -1) {
         perror("write");
      } else {
         buf += res;
         len -= res;
      }
   }
}

int main(int argc, char **argv) {
   char buf[2048];

   struct sigaction sig_info;
   sigaction(SIGINT, nullptr, &sig_info);
   std::cout << "Flags: " << sig_info.sa_flags << std::endl;
   sig_info.sa_flags &= ~SA_RESTART; // not needed because sa_flags is 0
   std::cout << "Flags: " << sig_info.sa_flags << std::endl;
   sig_info.sa_handler = handler;
   sigaction(SIGINT, &sig_info, nullptr);

   while (true) {
      ssize_t res = read(0, buf, sizeof(buf));
      if (g_got_signal) {
         std::cout << "Got a signal!" << std::endl;
         g_got_signal = false;
      }
      if (res == -1) {
         perror("read");
      } else if (res == 0) {
         return 0;
      } else {
         retry_write(1, buf, res);
         if (g_got_signal) {
             std::cout << "Got a signal!" << std::endl;
             g_got_signal = false;
         }
      }
   }
}
