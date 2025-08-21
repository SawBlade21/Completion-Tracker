#include "Utils.hpp"
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
    if (time == "") return time;

    std::string format = Mod::get()->getSettingValue<std::string>("time-format");
    std::tm tm = {};
    std::istringstream ss(time);
    auto is12h = time.find("AM") != std::string::npos || time.find("PM") != std::string::npos;
    
    if (format == "12-hour") {
        if (is12h) return time;

        ss >> std::get_time(&tm, "%H:%M:%S");
        if (ss.fail()) {
            return time + " AM";
        }
        else {
            std::ostringstream out;
            out << std::put_time(&tm, "%I:%M:%S %p");
            return out.str();
        }
        
    }
    else {
        if (!is12h) return time;
        
        ss >> std::get_time(&tm, "%I:%M:%S %p");

        if (ss.fail()) {
            return time.erase(time.length() - 3);
        }
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
        return time;
    }
    else {
        std::ostringstream out;
        out << std::put_time(&tm, "%H:%M:%S");
        return out.str();
    }
}

std::optional<std::chrono::system_clock::time_point> Utils::convertDate(std::string date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::time_t timeVal = std::mktime(&tm);
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(timeVal);

    if (ss.fail()) return std::nullopt;
    else return tp;
}

std::string Utils::isAMorPM(std::string time) {
    if (time.find("AM") != std::string::npos) return "AM";
    if (time.find("PM") != std::string::npos) return "PM";
    return "NA";
}
