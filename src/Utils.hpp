#pragma once

using namespace geode::prelude;

class Utils {

    public:

        /*const std::map<std::string, IconType> iconTypes {
            {"cube", IconType::Cube},
            {"ship", IconType::Ship},
            {"ball", IconType::Ball},
            {"ufo", IconType::Ufo},
            {"wave", IconType::Wave},
            {"robot", IconType::Robot},
            {"spider", IconType::Spider},
            {"swing", IconType::Swing},
            {"jetpack", IconType::Jetpack},
        };

        const std::map<IconType, std::string> iconStrings {
            {IconType::Cube, "cube"},
            {IconType::Ship, "ship"},
            {IconType::Ball, "ball"},
            {IconType::Ufo, "ufo"},
            {IconType::Wave, "wave"},
            {IconType::Robot, "robot"},
            {IconType::Spider, "spider"},
            {IconType::Swing, "swing"},
            {IconType::Jetpack, "jetpack"},
        };*/

        static int getIconFrame(IconType);

        static std::vector<std::string> splitString(const std::string& str, char delimiter);

        static std::string getTime(std::string time);

        static std::string isAMorPM(std::string time);
        
};
