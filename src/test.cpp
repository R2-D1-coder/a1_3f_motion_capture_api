#include "Mpro.h"
#include <thread>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv)
{
	ofstream ofs;

	time_t rawtime;
	struct tm* info;
	char buffer[80];
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(buffer, 80, "route_%m_%d_%H_%M_%S.csv", info);
	ofs.open(buffer, ios::out);

	Mpro app;
	app.Init();
	app.Start();
	bool isExit = false;
	std::thread thr([&]
		{
			std::list<HPP::RigidInfo> lis;
			bool first_time = false;
			//std::chrono::steady_clock::time_point start;

			std::chrono::time_point<std::chrono::system_clock> start;
			long long stamp_start = 0;

			while (!isExit)
			{
				app.GetResult(lis);
				if (lis.empty())
				{
					continue;
				}

				for (auto iter = lis.begin(); iter != lis.end(); ++iter)
				{

					if (first_time == false)
					{
						first_time = true;
						start = std::chrono::system_clock::now();
						stamp_start = iter->time_stamp;
					}
					auto now = start + std::chrono::microseconds(iter->time_stamp - stamp_start);
					auto now_log = now.time_since_epoch();
					typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<8>>::type> Days; /* UTC: +8:00 */
					Days days = std::chrono::duration_cast<Days>(now_log);
					now_log -= days;
					auto hours = std::chrono::duration_cast<std::chrono::hours>(now_log);
					now_log -= hours;
					auto minutes = std::chrono::duration_cast<std::chrono::minutes>(now_log);
					now_log -= minutes;
					auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now_log);
					now_log -= seconds;
					auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now_log);
					now_log -= milliseconds;
					auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now_log);
					now_log -= microseconds;
					auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now_log);
					ofs.fill('0');					ofs.width(2);
					ofs << hours.count() + 8 << ":";
					ofs.fill('0');					ofs.width(2);
					ofs << minutes.count() << ":";
					std::cout << minutes.count() << ":";
					ofs.fill('0');					ofs.width(2);
					ofs << seconds.count() << ".";
					std::cout << seconds.count() << ".";
					ofs.fill('0');					ofs.width(3);
					ofs << milliseconds.count() << "'";
					std::cout << milliseconds.count() << "'";
					ofs.fill('0');					ofs.width(3);
					ofs << microseconds.count() << "''";
					std::cout << microseconds.count() << "''";
					ofs.fill('0');					ofs.width(1);
					//ofs << nanoseconds.count() << "," << std::endl;
					//std::cout << nanoseconds.count() << "," << std::endl;
					ofs << 0 << "," ;
					std::cout << 0 << std::endl;

					ofs << iter->vel[0] << ", ";
					ofs << iter->vel[1] << ", ";
					ofs << iter->vel[2] << ", ";

					ofs << iter->angular_vel[0] << ", ";
					ofs << iter->angular_vel[1] << ", ";
					ofs << iter->angular_vel[2] << ", " << std::endl;
				}
				// std::this_thread::sleep_for(std::chrono::microseconds(1));
				// printf("list size:%zd\n", lis.size());
			}
			ofs.close(); });
	std::thread thr1([&]
		{
			getchar();
			isExit = true; });

	printf("wait............\n");
	thr.join();
	thr1.join();
	app.Deinit();

	return 0;
}
