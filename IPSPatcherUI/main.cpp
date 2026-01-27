#include "stdafx.h"
#include "IPSPatcherUI.h"
#include "IPSPatcherCore.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    // We can also expect the file to be patched, the IPS file, and optionally 
    // the output file path
    if (argc == 4)
    {
        const std::string filePath = argv[1];
        const std::string ipsPath = argv[2];
        const std::string outPath = argv[3];

        if (PatchFile(filePath, ipsPath, outPath))
        {
            printf("Patching completed successfully.");
            return 1;
        }
        else
        {
			printf("Patching failed.");
            return 0;
        }
    }

    // GUI mode
    QApplication app(argc, argv);
    IPSPatcherUI window;
    window.show();
    return app.exec();
}
