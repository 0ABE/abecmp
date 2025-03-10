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

#include "File.h"

namespace AbeCmp {

const static char lf_le[] = { '\n', '\0' };
const static char crlf_le[] = { '\r', '\n', '\0' };
const static char unknown_le[] = { '\0' };

const static char* pretty_lf = "\\n";
const static char* pretty_crlf = "\\r\\n";
const static char* pretty_unknown = "\\0";

File::~File()
{
    close();
}

bool
File::close()
{
    if (_file.is_open())
        _file.close();
    return !_file.is_open();
}

const char*
File::getPrettyLE() const
{
    if (_line_ending.second == lf_le)
        return pretty_lf;
    else if (_line_ending.second == crlf_le)
        return pretty_crlf;
    else
        return pretty_unknown;
}

bool
AbeCmp::File::initLE()
{
    char le_buf[3] = { '\0', '\0', '\0' };
    char c = '\0';
    while (!_file.eof()) {
        // Read the file char by char.
        _file.get(c);
        // Fill up the line ending buffer...
        le_buf[0] = le_buf[1];
        // left shifting the buffer contents with each read...
        le_buf[1] = c;
        // until the buffer is holding a line ending.
        if (le_buf[0] == crlf_le[0] && le_buf[1] == crlf_le[1]) {
            // Matches crlf.
            resetCursor();
            _line_ending = { "dos |crlf", crlf_le };
            return true;
        }
        if (le_buf[1] == lf_le[0]) {
            // Matches lf.
            resetCursor();
            _line_ending = { "unix|  lf", lf_le };
            return true;
        }
    }
    // There was no match to a known line ending.
    _line_ending = { "unknown", unknown_le };
    return false;
}

void
AbeCmp::File::initLineCount()
{
    _line_count = 0;
    std::string line_buf = {};
    // Read each line into the temp line_buf, based on the the line ending that was found.
    while (getline(_file, line_buf, *_line_ending.second)) {
        // Count the lines with each line read.
        ++_line_count;
        // Don't double count lines.
        skipCRLF();
    }
    resetCursor();
}

bool
AbeCmp::File::open(const std::string& name)
{
    _name = name;
    _file.open(name, std::ios::in);
    if (_file.is_open())
        if (initLE())
            initLineCount();
        else
            return close();

    return _file.is_open();
}

void
AbeCmp::File::resetCursor()
{
    if (!_file.is_open())
        return;

    _file.clear();
    _file.seekg(0, std::ios::beg);
}

void
AbeCmp::File::skipCRLF()
{
    if (_line_ending.second == crlf_le) {
        // Peek for a LF after a CR.
        char p = _file.peek();
        if (p == *lf_le) {
            // Found it, throw it away.
            _file.get(p);
        }
    }
}

std::string
AbeCmp::File::readLine(bool with_pretty_le)
{
    // Read until the known line ending, filling the member _current_line.
    std::string line_buf = {};
    getline(_file, line_buf, *_line_ending.second);
    // Don't double count lines.
    skipCRLF();
    if (with_pretty_le)
        return line_buf + getPrettyLE();
    else
        return line_buf;
}

} // namespace AbeCmp
