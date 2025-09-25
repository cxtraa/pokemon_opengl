
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

class Utils {
public:
    template <typename T>
    static T getProperty(json object, std::string propertyName) {
        std::vector<json> properties = object["properties"].get<std::vector<json>>();
        for (auto& prop : properties) {
            std::string name = prop["name"].get<std::string>();
            if (name == propertyName) {
                return prop["value"].get<T>();
            }
        }
        throw std::runtime_error("Utils: unable to find the property with name: " + propertyName);
    }

    static float randFloat();
};