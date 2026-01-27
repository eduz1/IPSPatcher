#include "stdafx.h"
#include "IPSPatcherUI.h"
#include "IPSPatcherCore.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    // We can also expect the file to be patched, the IPS file, and optionally 
    // the output file path
    if (argc == 3 || argc == 4)
    {
        const std::string filePath = argv[1];
        const std::string ipsPath = argv[2];
        std::string outPath = "";

        if (argc == 3)
        {
            outPath = filePath;
        }
        else
        {
			outPath = argv[3];
		}

        bool result = false;

		// If no output path is given, overwrite the input file
        if (outPath.empty())
            result = PatchFile(filePath, ipsPath, filePath);
        else
            result = PatchFile(filePath, ipsPath, outPath);

        if (result)
        {
            printf("Patching completed successfully.\n");
            std::fflush(NULL);
            return 0;
        }
        else
        {
			printf("Patching failed.\n");
            std::fflush(NULL);
            return 1;
        }
    }

    // GUI mode
    QApplication app(argc, argv);
    IPSPatcherUI window;
    window.show();
    return app.exec();
}
