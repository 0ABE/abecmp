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

#pragma once

// System includes.
#include <fstream>
#include <string>

namespace AbeCmp {

class File
{
  public:
    File() = default;
    ~File();

    // Close the file.
    bool close();
    long getLineCount() const { return _line_count; }
    std::string getLineEnding() const { return _line_ending.first; }
    std::string getName() const { return _name; }
    const char* getPrettyLE() const;
    // Open the file (sets the path/name).
    bool open(const std::string& name);
    // Read a line and store it in current_line.
    std::string readLine(bool with_pretty_le = true);

  private:
    // Init the line_ending member.
    bool initLE();
    // Init the line_count member.
    void initLineCount();
    // Reset the cursor and go back to the file beginnings.
    void resetCursor();
    // Skip a whole CRLF ending to avoid double counting the LF in CRLF.
    void skipCRLF();

  private:
    // Object to open and read lines in the file.
    std::ifstream _file;
    // Count the lines in each file. They have to be the same.
    long _line_count = 0;
    // Store the line ending information to use for this file.
    std::pair<std::string, const char*> _line_ending;
    // Store the path/name of the file.
    std::string _name = {};
};

} // namespace AbeCmp
