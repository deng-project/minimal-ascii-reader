// mar: Minimal ASCII reader for C++
// licence: Apache, see LICENCE file
// file: AsciiLineReader.h - AsciiLineReader class header
// author: Karl-Mihkel Ott

#ifndef ASCII_LINE_READER_H
#define ASCII_LINE_READER_H

#ifdef ASCII_LINE_READER_CPP
    #include <cstring>
    #include <string>
    #include <vector>
    #include <fstream>

    #include "mar/AsciiStreamReader.h"
#endif

namespace MAR {

    class AsciiLineReader : public AsciiStreamReader {
        private:
            char *m_line_beg = nullptr;
            char *m_line_end = nullptr;
            char *m_rd_ptr = nullptr;

        protected:
            /** 
             * Read new line from buffer
             * @return True if newline was found, false otherwise
             */
            bool _NextLine();
            /**
             * Read all general statement arguments until the end of line,
             * assuming that the first word of the line was read as a keyword
             */
            std::vector<std::string> _ReadStatementArgs();
            /**
             * Skip all skippable characters (' ', 0x00, '\t', '\d' and or '\n')
             * @param _skip_nl is a boolean flag indicating newline skipping
             */
            void _SkipSkippableCharacters(bool _skip_nl = false);
            /**
             * Skip the reading pointer by specified amount
             * @param _skip_val is a size_t value that specifies the read_ptr offset in bytes
             * @return true if data skipping was successful, false otherwise
             */
            bool _SkipData(const size_t _skip_val);
            /**
             * Extract word statement, assuming that the starting position character is not skippable
             * @return End position of the word
             */
            char *_ExtractWord();
            /**
             * Extract string value from buffer (value must be in between quotation marks)
             * @return std::string value that contains the string value without quotation marks
             */
            std::string _ExtractString();
            /**
             * Allocate memory and extract a binary blob from stream to buffer
             * @param _size specifying the readable blob size in bytes
             * @param _data is an optional argument specifying a pointer that specifies the memory area, 
             * where data should be written to to. If specified, no allocation occurs and the same pointer is returned
             * @return a pointer to the BLOB data memory area or nullptr if the reading failed
             */
            char *_ExtractBlob(uint32_t _size, char *_data = nullptr);
            /**
             * Get the current buffer reading pointer
             * @return current reading pointer
             */
            char *_GetReadPtr();
            /**
             * @return std::pair object where the first element specifies beginning of the line and second element
             * specifies the end of line
             */
            std::pair<char*, char*> _GetLineBounds();
            /**
             * Explicitly set the new m_rd_ptr value
             * @param _ptr, new m_rd_ptr value
             */
            void _SetReadPtr(char *_ptr);
            /**
             * Explicitly set the line bounds
             * @param _bounds is a std::pair value, where the first element states line beginning and second element states the line ending
             */
            void _SetLineBounds(const std::pair<char*, char*> &_bounds);

        public:
            AsciiLineReader(const std::string &_file_name, size_t _chunk_size = DEFAULT_CHUNK, const std::string &_end = "\n");
            AsciiLineReader(AsciiLineReader &&_alr) noexcept;
    };
}

#endif
