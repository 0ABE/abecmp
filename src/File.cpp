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
    if (m_file.is_open())
        m_file.close();
    return !m_file.is_open();
}

const char*
File::getPrettyLE() const
{
    if (m_line_ending.second == lf_le)
        return pretty_lf;
    else if (m_line_ending.second == crlf_le)
        return pretty_crlf;
    else
        return pretty_unknown;
}

// @brief Initialize the line ending member by reading the file char by char.
//        If a line ending is found, reset the cursor and return true.
//        If no line ending is found, set the line ending to unknown and return false.
//        This is used to determine the line ending type of the file.
bool
File::initLE()
{
    char le_buf[3] = { '\0', '\0', '\0' };
    char c = '\0';
    while (!m_file.eof()) {
        // Read the file char by char.
        m_file.get(c);
        // Fill up the line ending buffer...
        le_buf[0] = le_buf[1];
        // left shifting the buffer contents with each read...
        le_buf[1] = c;
        // until the buffer is holding a line ending.
        if (le_buf[0] == crlf_le[0] && le_buf[1] == crlf_le[1]) {
            // Matches crlf.
            resetCursor();
            m_line_ending = { "dos |crlf", crlf_le };
            return true;
        }
        if (le_buf[1] == lf_le[0]) {
            // Matches lf.
            resetCursor();
            m_line_ending = { "unix|  lf", lf_le };
            return true;
        }
    }
    // There was no match to a known line ending.
    m_line_ending = { "unknown", unknown_le };
    return false;
}

// @brief Initialize the line_count member by reading the file line by line.
//        This is used to count the lines in the file.
//        It also resets the cursor to the beginning of the file.
//        This is called after the line ending is determined.
void
File::initLineCount()
{
    m_line_count = 0;
    std::string line_buf = {};
    // Read each line into the temp line_buf, based on the the line ending that was found.
    while (getline(m_file, line_buf, *m_line_ending.second)) {
        // Count the lines with each line read.
        ++m_line_count;
        // Don't double count lines.
        skipCRLF();
    }
    resetCursor();
}

bool
File::open(const std::string& name)
{
    m_name = name;
    m_file.open(name, std::ios::in);
    if (m_file.is_open())
        if (initLE())
            initLineCount();
        else
            return close();

    return m_file.is_open();
}

void
File::resetCursor()
{
    if (!m_file.is_open())
        return;

    m_file.clear();
    m_file.seekg(0, std::ios::beg);
}

// @brief Skip a whole CRLF ending to avoid double counting the LF in CRLF.
//        This is only needed for CRLF line endings.
//        If the line ending is LF, this does nothing.
void
File::skipCRLF()
{
    if (m_line_ending.second == crlf_le) {
        // Peek for a LF after a CR.
        char p = m_file.peek();
        if (p == *lf_le) {
            // Found it, throw it away.
            m_file.get(p);
        }
    }
}

// @brief Read a line and return it as a string.
std::string
File::readLine(bool with_pretty_le)
{
    // Read until the known line ending, filling the line buffer.
    // If with_pretty_le is true, append the pretty line ending to the line buffer.
    std::string line_buf = {};
    getline(m_file, line_buf, *m_line_ending.second);
    // Don't double count lines.
    skipCRLF();
    if (with_pretty_le)
        return line_buf + getPrettyLE();
    else
        return line_buf;
}

} // namespace AbeCmp
