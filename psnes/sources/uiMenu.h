//
// Created by cpasjuste on 01/06/18.
//

#ifndef PSNES_UIMENU_H
#define PSNES_UIMENU_H

class PSNESUIMenu : public c2dui::UiMenu {

public:

    PSNESUIMenu(c2dui::UiMain *ui);

    bool isOptionHidden(c2dui::Option *option);

};

#endif //PSNES_UIMENU_H
