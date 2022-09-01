#pragma once
#include "define/HppGeometryDef.h"

#define MAX_RIGID 3
namespace HPP
{

    enum poseStates
    {
        STATES_TRACKING = 0,
        STATES_INITIAL = 1,
        STATES_PREDICTION = 2,
        STATES_LOST = 3
    };

      struct RigidPose
      {
            /// quaternion := w,x,y,z;
        double quaternion[4] = {1,0,0,0};

            /// translation := tx ty tz;
        double translation[3] = {0};
      };

      struct RigidInfo
      {
            int id;
            RigidPose pose;//pose
            double euler[3] = { 0 };
            Point3dSet points_in_global_reference;
            double vel[3];//线速度
            double angular_vel[3];//角速度
            long long time_stamp =0;//时间戳
            poseStates states;//pose状态
      };

      struct RigidInfoSet
      {
            RigidInfo poseInfo[MAX_RIGID];
            int numInfo;
      };

}