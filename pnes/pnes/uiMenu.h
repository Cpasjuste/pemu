//
// Created by cpasjuste on 01/06/18.
//

#ifndef PNES_UIMENU_H
#define PNES_UIMENU_H

class PNESGuiMenu : public c2dui::C2DUIGuiMenu {

public:

    PNESGuiMenu(c2dui::C2DUIGuiMain *ui);

    bool isOptionHidden(c2dui::C2DUIOption *option);

};

#endif //PNES_UIMENU_H
