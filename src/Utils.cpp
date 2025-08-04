#include "Utils.hpp"
#include <regex>
#include <chrono>

using namespace geode::prelude;

int Utils::getIconFrame(IconType type) {
    GameManager* gm = GameManager::get();

    switch (type) {
        case IconType::Cube: return gm->getPlayerFrame();
        case IconType::Ship: return gm->getPlayerShip();
        case IconType::Ball: return gm->getPlayerBall();
        case IconType::Ufo: return gm->getPlayerBird();
        case IconType::Wave: return gm->getPlayerDart();
        case IconType::Robot: return gm->getPlayerRobot();
        case IconType::Spider: return gm->getPlayerSpider();
        case IconType::Swing: return gm->getPlayerSwing();
        case IconType::Jetpack: return gm->getPlayerJetpack();
        default: return gm->getPlayerFrame();
    }
}

std::vector<std::string> Utils::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> splitString;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        splitString.push_back(item);
    }
    return splitString;
}

std::string Utils::getTime(std::string time) {
    if (time == "")
        return time;

    std::string format = Mod::get()->getSettingValue<std::string>("time-format");
    std::tm tm = {};
    std::istringstream ss(time);

    //std::regex ampmRegex("(AM|PM)", std::regex_constants::icase);
    auto is12h = time.find("AM") != std::string::npos || time.find("PM") != std::string::npos;

    log::debug("is12h: {}", is12h);
    
    if (format == "12-hour") {
        if (is12h)
            return time;

        ss >> std::get_time(&tm, "%H:%M:%S");
        if (ss.fail())
            return time;
        else {
            std::ostringstream out;
            out << std::put_time(&tm, "%I:%M:%S %p");
            return out.str();
        }
        
    }
    else {
        if (!is12h)
            return time;
        
        ss >> std::get_time(&tm, "%I:%M:%S %p");
        if (ss.fail())
            return time;
        else {
            std::ostringstream out;
            out << std::put_time(&tm, "%H:%M:%S");
            return out.str();
        }
    }
}

std::string Utils::convertTime(std::string time) {
    std::string newTime;
    std::tm tm = {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, "%I:%M:%S %p");
    if (ss.fail()) {
        log::debug("fail 1");
        return time;
    }
    else {
        std::ostringstream out;
        out << std::put_time(&tm, "%H:%M:%S");
        return out.str();
    }
}

std::optional<std::chrono::system_clock::time_point> Utils::convertDate(std::string date) {
    std::istringstream ss(date);
    std::chrono::system_clock::time_point tp;

    // Parse the string using a format specifier
    ss >> std::chrono::parse("%Y-%m-%d %H:%M:%S", tp);

    if (ss.fail()) {
        log::debug("fail 2");
        return std::nullopt;
    }
    else
        return tp;
}

std::string Utils::isAMorPM(std::string time) {
    log::debug("a");
    if (time.find("AM") != std::string::npos)
        return "AM";
    log::debug("b");
    if (time.find("PM") != std::string::npos)
        return "PM";
    log::debug("c");
    return "NA";
}
