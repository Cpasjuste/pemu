//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIMENU_H
#define PFBA_UIMENU_H

class PFBAGuiMenu : public c2dui::UiMenu {

public:

    explicit PFBAGuiMenu(c2dui::UiMain *ui) : UiMenu(ui) {};

    bool isOptionHidden(c2dui::Option *option) override;

};

#endif //PFBA_UIMENU_H
