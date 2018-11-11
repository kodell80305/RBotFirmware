// RBotFirmware
// Rob Dobson 2016-2018

#pragma once

#include "RdJson.h"

class RobotConfigurations
{
  public:
    static const char *_robotConfigs[];
    static const int _numRobotConfigurations;

    static const char *getConfig(const char *robotTypeName)
    {
        Log.notice("RobotConfigurations: Requesting %s, there are %d default types\n", robotTypeName, _numRobotConfigurations);
        for (int configIdx = 0; configIdx < _numRobotConfigurations; configIdx++)
        {
            String robotType = RdJson::getString("robotType", "", _robotConfigs[configIdx]);
            if (robotType.equals(robotTypeName))
            {
                Log.notice("RobotConfigurations: Config for %s found\n", robotTypeName);
                return _robotConfigs[configIdx];
            }
        }
        return "{}";
    }

    // Return list of robot types
    static const void getRobotConfigurations(String &retStr)
    {
        retStr = "[";
        for (int configIdx = 0; configIdx < _numRobotConfigurations; configIdx++)
        {
            String robotType = RdJson::getString("robotType", "", _robotConfigs[configIdx]);
            if (configIdx != 0)
                retStr += ",";
            retStr += "\"" + robotType + "\"";
        }
        retStr += "]";
    }

    // Get Nth robot type name
    static void getNthRobotTypeName(int n, String& robotTypeName)
    {
        if ((n < 0) || (n > _numRobotConfigurations))
            robotTypeName = "";
        robotTypeName = RdJson::getString("robotType", "", _robotConfigs[n]);
    }
};