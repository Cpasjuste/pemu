//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIMENU_H
#define PFBA_UIMENU_H

class PFBAGuiMenu : public c2dui::C2DUIGuiMenu {

public:

    PFBAGuiMenu(c2dui::C2DUIGuiMain *ui);

    bool isOptionHidden(c2dui::C2DUIOption *option);

};

#endif //PFBA_UIMENU_H
