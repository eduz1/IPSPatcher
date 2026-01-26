#pragma once

#include <QtWidgets/QWidget>
#include "ui_IPSPatcherUI.h"

class IPSPatcherUI : public QWidget
{
    Q_OBJECT

public:
    IPSPatcherUI(QWidget *parent = nullptr);
    ~IPSPatcherUI();

private:
    Ui::IPSPatcherUIClass ui;

    void OnBtnClickedFileBrowse();
    void OnBtnClickedIPSBrowse();
    
    void OnCheckClickedOverwrite();
    void OnBtnClickedOutBrowse();

    void OnBtnClickedConvert();
};

