#include <regex>
#include <stdexcept>
#include "strategy_creator.hpp"
#include "allc_strategy.hpp"
#include "alld_strategy.hpp"
#include "tft_strategy.hpp"
#include "grim_strategy.hpp"
#include "pavlov_strategy.hpp"
#include "rnd_strategy.hpp"

std::unique_ptr<Strategy> StrategyCreator::createStrategy(const std::string& stratName) {
    if (stratName == "ALLC") {
        return std::make_unique<ALLC>();
    }
    if (stratName == "ALLD") {
        return std::make_unique<ALLD>();
    }
    if (stratName == "TFT") {
        return std::make_unique<TFT>();
    }
    if (stratName == "GRIM") {
        return std::make_unique<GRIM>();
    }
    if (stratName == "PAVLOV") {
        return std::make_unique<PAVLOV>();
    }

    std::regex rndStratPattern(R"(RND([0-9]*\.?[0-9]+))");
    std::smatch match;

    if (std::regex_match(stratName, match, rndStratPattern)) {
        double probability = std::stod(match[1].str());
        
        return std::make_unique<RND>(probability);
    }

    throw std::invalid_argument("Invalid strategy: " + stratName);
}