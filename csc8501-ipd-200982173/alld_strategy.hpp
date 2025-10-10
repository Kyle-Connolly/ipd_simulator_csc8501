#pragma once

#include "strategy.hpp"

class ALLD : public Strategy {
public:
    Action decideAction() override;
    std::string name() const override;
};