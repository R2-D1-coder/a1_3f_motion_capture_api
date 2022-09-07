#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include "Mpro.h"

using namespace std;

int main(int argc, char** argv) {
  ofstream ofs;

  time_t     rawtime;
  struct tm* info;
  char       buffer[80];
  time(&rawtime);
  info = localtime(&rawtime);
  strftime(buffer, 80, "route_%m_%d_%H_%M_%S.csv", info);
  ofs.open(buffer, ios::out);

  Mpro app;
  app.Init();
  app.Start();
  bool        isExit = false;
  std::thread thr([&] {
    std::list<HPP::RigidInfo> lis;
    // bool                      first_time = false;
    //  std::chrono::steady_clock::time_point start;

    std::chrono::time_point<std::chrono::system_clock> start;
    long long                                          stamp_start = 0;

    while (!isExit) {
      app.GetResult(lis);
      if (lis.empty()) {
        continue;
      }
      bool first_time = false;
      for (auto iter = lis.begin(); iter != lis.end(); ++iter) {
        if (first_time == false) {
          first_time  = true;
          start       = std::chrono::system_clock::now();
          stamp_start = iter->time_stamp;
        }
        auto now     = start + std::chrono::microseconds(iter->time_stamp - stamp_start);
        auto now_log = now.time_since_epoch();
        typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8>>::type>
             Days; /* UTC: +8:00 */
        Days days = std::chrono::duration_cast<Days>(now_log);

        auto hours        = std::chrono::duration_cast<std::chrono::hours>(now_log -= days);
        auto minutes      = std::chrono::duration_cast<std::chrono::minutes>(now_log -= hours);
        auto seconds      = std::chrono::duration_cast<std::chrono::seconds>(now_log -= minutes);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now_log -= seconds);
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now_log -= milliseconds);
        auto nanoseconds  = std::chrono::duration_cast<std::chrono::nanoseconds>(now_log -= microseconds);
        ofs.fill('0');
        ofs.width(2);
        ofs << hours.count() + 8 << ":";
        ofs.width(2);
        ofs << minutes.count() << ":";
        ofs.width(2);
        ofs << seconds.count() << ".";
        ofs.width(3);
        ofs << milliseconds.count() << "'";
        ofs.width(3);
        ofs << microseconds.count() << "''" << 0 << ", ";
        ofs << setiosflags(ios::fixed) << setprecision(8);
        ofs << sqrt(pow(iter->vel[0], 2) + pow(iter->vel[1], 2) + pow(iter->vel[2], 2)) << ", ";
        ofs << iter->angular_vel[2] << "," << std::endl;

        std::cout << minutes.count() << ":" << seconds.count() << "." << milliseconds.count() << "'"
                  << microseconds.count() << "''" << 0 << ":  ";
        std::cout << iter->vel[0] << ", " << iter->vel[1] << ", " << iter->vel[2] << ", ";
        std::cout << iter->angular_vel[0] << ", " << iter->angular_vel[1] << ", " << iter->angular_vel[2] << ", "
                  << std::endl;
      }
      // std::this_thread::sleep_for(std::chrono::microseconds(1));
      // printf("list size:%zd\n", lis.size());
    }
    ofs.close();
  });
  std::thread thr1([&] {
    getchar();
    isExit = true;
  });

  printf("wait............\n");
  thr.join();
  thr1.join();
  app.Deinit();

  return 0;
}
