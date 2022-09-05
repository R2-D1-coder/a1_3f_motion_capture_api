#include "Mpro.h"
#include <thread>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	ofstream ofs;
	ofs.open("route_info.csv", ios::out);

	Mpro app;
	app.Init();
	app.Start();
	bool isExit = false;
	std::thread thr([&]
		{
			std::list<HPP::RigidInfo> lis;
			bool first_time = false;
			std::chrono::steady_clock::time_point start;
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
					ofs << iter->id << ", ";

					ofs << iter->pose.quaternion[0] << ", ";
					ofs << iter->pose.quaternion[1] << ", ";
					ofs << iter->pose.quaternion[2] << ", ";
					ofs << iter->pose.quaternion[3] << ", ";
					ofs << iter->pose.translation[0] << ", ";
					ofs << iter->pose.translation[1] << ", ";
					ofs << iter->pose.translation[2] << ", ";

					ofs << iter->euler[0] << ", ";
					ofs << iter->euler[1] << ", ";
					ofs << iter->euler[2] << ", ";

					ofs << iter->vel[0] << ", ";
					ofs << iter->vel[1] << ", ";
					ofs << iter->vel[2] << ", ";

					ofs << iter->angular_vel[0] << ", ";
					ofs << iter->angular_vel[1] << ", ";
					ofs << iter->angular_vel[2] << ", ";

					ofs << iter->time_stamp << ", ";

					if (first_time == false)
					{
						start = std::chrono::high_resolution_clock::now();
						stamp_start = iter->time_stamp;
					}
					auto now = start + std::chrono::nanoseconds(iter->time_stamp - stamp_start);
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

					ofs << hours.count() << ":"
						<< minutes.count() << ":"
						<< seconds.count() << "."
						<< milliseconds.count() << "'"
						<< microseconds.count() << "''"
						<< nanoseconds.count() << ","<< std::endl;
				

					printf("pose: %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f\n",
						iter->pose.quaternion[0],
						iter->pose.quaternion[1],
						iter->pose.quaternion[2],
						iter->pose.quaternion[3],
						iter->pose.translation[0],
						iter->pose.translation[1],
						iter->pose.translation[2]);
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
