//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIMENU_H
#define PFBA_UIMENU_H

class PFBAGuiMenu : public c2dui::UIMenuNew {

public:

    explicit PFBAGuiMenu(c2dui::UIMain *ui) : UIMenuNew(ui) {};

    bool isOptionHidden(c2dui::Option *option) override;

};

#endif //PFBA_UIMENU_H
