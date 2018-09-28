//
// Created by cpasjuste on 01/06/18.
//

#ifndef PNES_UIMENU_H
#define PNES_UIMENU_H

class PNESGuiMenu : public c2dui::UIMenu {

public:

    PNESGuiMenu(c2dui::UIMain *ui);

    bool isOptionHidden(c2dui::Option *option);

};

#endif //PNES_UIMENU_H
