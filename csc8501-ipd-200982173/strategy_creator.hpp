#pragma once
#include <memory>
#include <string>
#include <vector>
#include "strategy.hpp"

class StrategyCreator {
public:
    static std::unique_ptr<Strategy> createStrategy(const std::string& name);
};