#pragma once

#include "strategy.hpp"

class TFT : public Strategy {
public:
    Action decideAction() override;
    std::string name() const override;
};