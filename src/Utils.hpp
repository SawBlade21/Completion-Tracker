#pragma once

using namespace geode::prelude;

class Utils {

    public:

        static int getIconFrame(IconType);

        static std::vector<std::string> splitString(const std::string& str, char delimiter);

        static std::string getTime(std::string time);

        static std::string isAMorPM(std::string time);

        static std::string convertTime(std::string time);
        
        static std::optional<std::chrono::system_clock::time_point> convertDate(std::string date);
        
};
