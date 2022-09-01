#include "Mpro.h"
#include <thread>
#include <iostream>
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
    std::thread thr([&] {
        std::list<HPP::RigidInfo> lis;
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

                ofs << iter->states << std::endl;

                printf("pose: %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f\n",
                    iter->pose.quaternion[0],
                    iter->pose.quaternion[1],
                    iter->pose.quaternion[2],
                    iter->pose.quaternion[3],
                    iter->pose.translation[0],
                    iter->pose.translation[1],
                    iter->pose.translation[2]
                );
            }
            //std::this_thread::sleep_for(std::chrono::microseconds(1));
            //printf("list size:%zd\n", lis.size());
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
