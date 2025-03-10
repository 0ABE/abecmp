/**
 *        d8888 888                .d8888b.
 *       d88888 888               d88P  Y88b
 *      d88P888 888               888    888
 *     d88P 888 88888b.   .d88b.  888        88888b.d88b.  88888b.
 *    d88P  888 888 "88b d8P  Y8b 888        888 "888 "88b 888 "88b
 *   d88P   888 888  888 88888888 888    888 888  888  888 888  888
 *  d8888888888 888 d88P Y8b.     Y88b  d88P 888  888  888 888 d88P
 * d88P     888 88888P"   "Y8888   "Y8888P"  888  888  888 88888P"
 *                                                         888
 * A simple file comparison tool                           888
 *                                                         888
 * Copyright (c) 2025, Abe Mishler
 * Licensed under the Universal Permissive License v 1.0
 * as shown at https://oss.oracle.com/licenses/upl/.
 */

// Project includes
#include "AbeCmpConfig.h"
#include "File.h"
#include "Platform.h"

// AbeArgs command line processing.
#include <abeargs/abeargs.h>

// System includes
#include <cstring>
#include <fstream>
#include <iostream>

void
showAbout(int lines = 2)
{
    printf("AbeCmp v%s (%s %s), %s %s\n", ABECMP_VERSION, AbeCmp::getArch(), AbeCmp::getHostType(), __DATE__, __TIME__);
    if (lines == 1)
        return;

    printf("A simple file comparison tool.\n");
}

void
showHelp(const AbeArgs::Parser& parser)
{
    showAbout();
    printf("\nHelp:\n");

    AbeArgs::ArgumentList_t list = parser.getArguments();
    for (size_t i = 0, n = list.size(); i < n; ++i) {
        std::cout << list[i].toString();
        printf("\n");

        if (i < n - 1 && list[i].hasDefaultValue())
            printf("\n");
    }
    printf("\n");
}

int
main(int argc, char** argv)
{
    AbeCmp::File file_a, file_b;

    const int FILE_A_ID = 1;  // File a.
    const int FILE_B_ID = 2;  // File b.
    const int FILE_IG_ID = 3; // Ignore differences in line endings.
    const int QUIET_ID = 4;   // Don't print out any differences.
    const int VERSION_ID = 5; // Print out version/about information.
    const int HELP_ID = 6;    // Print out usage help.

    AbeArgs::Parser parser;
    parser.addArgument({ AbeArgs::REQUIRED, FILE_A_ID, "a", "file-a", "File a to compare.", AbeArgs::FILE_t, 1 });
    parser.addArgument({ AbeArgs::REQUIRED, FILE_B_ID, "b", "file-b", "File b to compare.", AbeArgs::FILE_t, 1 });
    parser.addArgument({ AbeArgs::SWITCH, FILE_IG_ID, "i", "ignore", "Ignore differences in line endings." });
    parser.addArgument({ AbeArgs::SWITCH, QUIET_ID, "q", "quiet", "Only print the final result." });
    parser.addArgument({ AbeArgs::X_SWITCH, VERSION_ID, "v", "version", "Show version information and exit." });
    parser.addArgument({ AbeArgs::X_SWITCH, HELP_ID, "h", "help", "Show this help information and exit." });

    // Set initial default values.
    bool quiet = false;
    parser.getArgument(QUIET_ID).setDefaultValue(quiet);
    bool le_ignore = false;
    parser.getArgument(FILE_IG_ID).setDefaultValue(le_ignore);

    AbeArgs::ParsedArguments_t results = parser.exec(argc, argv);
    if (parser.error()) {
        showAbout(1);
        std::cerr << "\n"
                  << parser.getErrorMsg() << "\n";
        return EXIT_FAILURE;
    } else {
        for (const auto& r : results) {
            switch (r.first) {
                case FILE_A_ID:
                    if (!file_a.open(std::get<std::string>(r.second))) {
                        std::cerr << "\nerror: Opening file: " << file_a.getName() << "\n";
                        return EXIT_FAILURE;
                    }
                    break;
                case FILE_B_ID:
                    if (!file_b.open(std::get<std::string>(r.second))) {
                        std::cerr << "\nerror: Opening file: " << file_b.getName() << "\n";
                        return EXIT_FAILURE;
                    }
                    break;
                case FILE_IG_ID:
                    le_ignore = std::get<bool>(r.second);
                    break;
                case QUIET_ID:
                    quiet = std::get<bool>(r.second);
                    break;
                case VERSION_ID:
                    showAbout();
                    return EXIT_SUCCESS;
                case HELP_ID:
                    showHelp(parser);
                    return EXIT_SUCCESS;
            }
        }
    }

    showAbout(1);

    if (parser.isMissingRequiredArgs()) {
        std::cerr << "\nerror: Missing 1 or more required arguments.";
        std::cerr << "\n       Use -h or --help for more information.\n";
        return EXIT_FAILURE;
    }

    const bool diff_line_endings = (file_a.getLineEnding() != file_b.getLineEnding());
    std::cout << "\nFile a [" << file_a.getLineEnding() << "]: " << file_a.getName();
    std::cout << "\nFile b [" << file_b.getLineEnding() << "]: " << file_b.getName() << "\n";
    if (diff_line_endings)
        printf("The files use different line endings.\n");

    if (le_ignore)
        // Ignore line endings/differences.
        // Use what each file has for a line ending.
        printf("Ignoring line ending differences.\n");

    // Line count test.
    if (file_a.getLineCount() != file_b.getLineCount()) {
        printf("\nThe files are not the same because the line counts do not match.");
        std::cout << "\nFile a: " << file_a.getLineCount() << (file_a.getLineCount() == 1 ? " line." : " lines.");
        std::cout << "\nFile b: " << file_b.getLineCount() << (file_b.getLineCount() == 1 ? " line.\n" : " lines.\n");
        return EXIT_SUCCESS;
    }

    // Once the line counts are the same, compare each file line by line.
    bool the_same = true;
    long line_diffs = 0;
    long line_count = 0;
    std::string line_a, line_b;
    for (long i = 0; i < file_a.getLineCount(); ++i) {
        line_a = file_a.readLine(diff_line_endings && !le_ignore);
        line_b = file_b.readLine(diff_line_endings && !le_ignore);
        ++line_count;
        if (line_a.compare(line_b) != 0) {
            if (the_same && !quiet)
                printf("\n");

            if (!quiet) {
                printf("@@ %ld, line %ld\n", line_diffs + 1, i + 1);
                std::cout << "File a: " << line_a << "\n";
                std::cout << "File b: " << line_b << "\n";
                printf("\n");
            }
            the_same = false;
            line_diffs++;
        }
    }

    if (quiet)
        printf("\n");

    // Report the results.
    if (the_same) {
        if (diff_line_endings)
            printf("\nThe file contents match, but they have different line endings.\n");
        else
            printf("\nThe files match.\n");

        printf("%ld lines were compared.\n", line_count);
    } else {
        printf("The files don't match.\n");
        printf("%ld of %ld lines were different.\n", line_diffs, line_count);
    }

    return EXIT_SUCCESS;
}
