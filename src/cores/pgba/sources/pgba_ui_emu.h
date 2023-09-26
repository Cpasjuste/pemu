//
// Created by cpasjuste on 01/06/18.
//

#ifndef PGBA_UI_EMU_H
#define PGBA_UI_EMU_H

#include <string>
#include "mgba/core/core.h"

class PGBAUiEmu : public pemu::UiEmu {
public:
    explicit PGBAUiEmu(pemu::UiMain *ui);

    int load(const ss_api::Game &game) override;

private:
    void stop() override;

    void onUpdate() override;
};

#endif //PGBA_UI_EMU_H
