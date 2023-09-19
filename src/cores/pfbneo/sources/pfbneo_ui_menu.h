//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIMENU_H
#define PFBA_UIMENU_H

class PFBAGuiMenu : public pemu::UiMenu {
public:
    explicit PFBAGuiMenu(pemu::UiMain *ui) : UiMenu(ui) {};

    bool isOptionHidden(c2d::config::Option *option) override;

};

#endif //PFBA_UIMENU_H
