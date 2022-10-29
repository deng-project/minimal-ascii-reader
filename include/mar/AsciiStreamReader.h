// mar: Minimal ASCII reader for C++
// licence: Apache, see LICENCE file
// file: AsciiStreamReader.h - AsciiStreamReader class header
// author: Karl-Mihkel Ott

#ifndef ASCII_STREAM_READER_H
#define ASCII_STREAM_READER_H

#ifdef ASCII_STREAM_READER_CPP
    #include <cstring>
    #include <string>
    #include <vector>
    #include <fstream>

    #include "mar/MARAssert.h"
#endif

#define DEFAULT_CHUNK 4096

namespace MAR {

    class AsciiStreamReader {
        private:
            const std::string m_file_name;
            const std::string m_end;    // end string to use for buffering
            std::ifstream m_stream;

        protected:
            char *m_buffer = nullptr;
            size_t m_buffer_size;
            size_t m_last_read = 0;
            size_t m_stream_size = 0;

        private:
            std::vector<size_t> _KMP_FindEndStringInstances();  // using KMP algorithm

        protected:
            AsciiStreamReader(const std::string &_file_name = "", size_t _chunk_size = DEFAULT_CHUNK, const std::string &_end = "\n");
            AsciiStreamReader(AsciiStreamReader &&_asr) noexcept;
            ~AsciiStreamReader();
            /**
             * Read new buffer chunk from the stream
             * @return true if new chunk value was read
             */
            bool _ReadNewChunk(int64_t _scrollback = 0);
            /**
             * Get the current stream reading std::ifstream reference 
             * @return reference to std::ifstream 
             */
            std::ifstream &_GetStream();
            /**
             * Open new file stream for reading data from
             * @param _file_name is an std::string referece value that represents the used file name
             */
            void NewFile(const std::string &_file_name);
            /**
             * Close previously opened file stream if possible
             */
            void CloseFile();
            /**
             * Read custom binary blob from seemingly binary file
             * @param _cont is a reference to the data structure where binary data is read into
             */
            template<typename T>
            void ReadBlob(T &_cont) {
                m_stream.read(reinterpret_cast<char*>(&_cont), sizeof(T));
            }
            /**
             * Get the chunk buffer starting address
             * @return Starting address of the chunk buffer
             */
            char *GetBufferAddress();
            /**
             * Get the current open file name
             * @return std::string instance containing the file name
             */
            std::string GetFileName();
    };
}

#endif
