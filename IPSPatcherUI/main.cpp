#include "stdafx.h"
#include "IPSPatcherUI.h"
#include "IPSPatcherCore.h"

#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char* argv[])
{
    // Use Q(Core)Application just for parsing first
    QCoreApplication coreApp(argc, argv);
    QCoreApplication::setApplicationName("IPSPatcher");

    QCommandLineParser parser;
    parser.setApplicationDescription("IPS patcher (CLI and GUI)");
    parser.addHelpOption();
    parser.addVersionOption();

    // Positional arguments: input file, patch file, optional output file
    parser.addPositionalArgument("input",  "File to be patched");
    parser.addPositionalArgument("patch",  "IPS patch file");
    parser.addPositionalArgument("output", "Optional output file (defaults to input)", "[output]");

    parser.process(coreApp);

    const QStringList positionalArgs = parser.positionalArguments();

    // No positional args -> start GUI mode
    if (positionalArgs.isEmpty())
    {
        QApplication app(argc, argv);
        IPSPatcherUI window;
        window.show();
        return app.exec();
    }

    // We expect 2 or 3 positional args for CLI mode
    if (positionalArgs.size() < 2 || positionalArgs.size() > 3)
    {
        parser.showHelp(1); // exits with code 1
    }

    const std::string filePath = positionalArgs.at(0).toStdString();
    const std::string ipsPath  = positionalArgs.at(1).toStdString();
	// If no output path is given, overwrite the input file
    const std::string outPath  = (positionalArgs.size() == 3) ? positionalArgs.at(2).toStdString() : filePath;

    // If no output path is given, overwrite the input file (same behavior as before)
    const bool result = PatchFile(filePath, ipsPath, outPath);

    qInfo() << (result ? "Patching completed successfully." : "Patching failed.");
    return result ? 0 : 1;
}
