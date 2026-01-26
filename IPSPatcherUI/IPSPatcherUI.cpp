#include "stdafx.h"
#include "IPSPatcherUI.h"
#include "IPSPatcherCore.h"

IPSPatcherUI::IPSPatcherUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.fileBrowseBtn, &QPushButton::clicked, this, &IPSPatcherUI::OnBtnClickedFileBrowse);
    connect(ui.ipsBrowseBtn, &QPushButton::clicked, this, &IPSPatcherUI::OnBtnClickedIPSBrowse);
    connect(ui.outBrowseBtn, &QPushButton::clicked, this, &IPSPatcherUI::OnBtnClickedOutBrowse);

    connect(ui.overwriteCheck, &QCheckBox::clicked, this, &IPSPatcherUI::OnCheckClickedOverwrite);
    connect(ui.convertBtn, &QPushButton::clicked, this, &IPSPatcherUI::OnBtnClickedConvert);
}

IPSPatcherUI::~IPSPatcherUI()
{}

// Handles browsing for the input file to be patched and updates the input path field
void IPSPatcherUI::OnBtnClickedFileBrowse()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Select file to patch"), "", tr("All Files (*)"));
	ui.filePath->setText(filename);
}

// Handles browsing for the IPS patch file and updates the IPS path field
void IPSPatcherUI::OnBtnClickedIPSBrowse()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select .ips file"), "", tr("IPS Files (*.ips)"));
    ui.ipsPath->setText(filename);
}

// Handles browsing for the output file and updates the output path field
void IPSPatcherUI::OnBtnClickedOutBrowse()
{
    // Write filename
	QString filename = QFileDialog::getSaveFileName(this, tr("Select output file"), "", tr("All Files (*)"));
    ui.outPath->setText(filename);
}

// Enables or disables the output path controls when the overwrite option changes
void IPSPatcherUI::OnCheckClickedOverwrite()
{
    if (ui.overwriteCheck->isChecked())
    {
        ui.outPath->setEnabled(false);
		ui.outBrowseBtn->setEnabled(false);
    }
    else
    {
        ui.outPath->setEnabled(true);
        ui.outBrowseBtn->setEnabled(true);
    }
}

// Starts the patching operation using the selected files
void IPSPatcherUI::OnBtnClickedConvert()
{
	QString outPath = ui.outPath->text();

    if (ui.overwriteCheck->isChecked())
    {
        outPath = ui.filePath->text();
	}

	if (PatchFile(ui.filePath->text().toStdString(), ui.ipsPath->text().toStdString(), outPath.toStdString()))
    {
        QMessageBox::information(this, tr("Success"), tr("File patched successfully!"));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to patch file."));
    }
}

