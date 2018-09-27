//
// Created by cpasjuste on 01/06/18.
//

#ifndef PSNES_UIMENU_H
#define PSNES_UIMENU_H

class PSNESGuiMenu : public c2dui::C2DUIGuiMenu {

public:

    PSNESGuiMenu(c2dui::C2DUIGuiMain *ui);

    bool isOptionHidden(c2dui::C2DUIOption *option);

};

#endif //PSNES_UIMENU_H
