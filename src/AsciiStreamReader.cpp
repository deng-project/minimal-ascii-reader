// mar: Minimal ASCII reader for C++
// licence: Apache, see LICENCE file
// file: AsciiStreamReader.cpp - AsciiStreamReader class implementation
// author: Karl-Mihkel Ott

#define ASCII_STREAM_READER_CPP
#include <AsciiStreamReader.h>


namespace MAR {

    AsciiStreamReader::AsciiStreamReader(const std::string &_file_name, size_t _chunk_size, const std::string &_end) : 
        m_file_name(_file_name),
        m_end(_end), 
        m_buffer_size(_chunk_size) 
    {
        MAR_ASSERT(_chunk_size > 0);

        if(_file_name != "") {
            NewFile(_file_name);
            _ReadNewChunk();
        }
    }


    AsciiStreamReader::AsciiStreamReader(AsciiStreamReader &&_asr) noexcept :
        m_file_name(std::move(_asr.m_file_name)),
        m_end(std::move(_asr.m_end)),
        m_stream(std::move(_asr.m_stream)),
        m_buffer(_asr.m_buffer),
        m_buffer_size(_asr.m_buffer_size),
        m_last_read(_asr.m_last_read),
        m_stream_size(_asr.m_stream_size) 
    {
        _asr.m_buffer = nullptr;
    }


    AsciiStreamReader::~AsciiStreamReader() {
        CloseFile();
    }


    std::vector<size_t> AsciiStreamReader::_KMP_FindEndStringInstances() {
        // create an lsp array
        std::vector<size_t> lsp(m_end.size());
        for(size_t i = 1, j = 0; i < m_end.size(); i++) {
            j = lsp[i - 1];

            // skip invalid prefixes
            while(j > 0 && m_end[i] != m_end[j])
                j = lsp[j - 1];

            if(m_end[i] == m_end[j])
                j++;

            lsp[i] = j;
        }

        std::vector<size_t> occurances;
        occurances.reserve(m_buffer_size / m_end.size() > 0 ? 
                           m_buffer_size / m_end.size() : 1); // assuming the worst case memory usage
        for(size_t i = 0, j = 0; i < m_end.size(); i++) {
            // fallback on invalid values
            while(j > 0 && m_buffer[i] != m_end[j])
                j = lsp[j - 1];

            if(m_buffer[i] == m_end[j]) {
                j++;
                if(j == m_end.size()) {
                    occurances.push_back(i - j + 1);
                    j = lsp[j - 1]; // fallback
                }
            }
        }   

        return occurances;
    }


    bool AsciiStreamReader::_ReadNewChunk() {
        // verify that stream is open
        MAR_ASSERT(m_stream.is_open());
            
        size_t read_bytes = m_stream.tellg();
        m_last_read = std::min(m_buffer_size, m_stream_size - static_cast<size_t>(read_bytes));

        if(m_last_read == 0) return false;

        // clear the buffer and refill it with new data from stream
        std::memset(m_buffer, 0, m_buffer_size);
        m_stream.read(m_buffer, m_last_read);

        read_bytes = m_stream.tellg();

        // using backwards KMP substring search algorithm
        std::vector<size_t> instances = _KMP_FindEndStringInstances();

        int64_t back = 0;
        if(instances.size()) {
            int64_t back = static_cast<int64_t>(instances.back()) + static_cast<int64_t>(m_end.size()) - 
                           static_cast<int64_t>(m_last_read);

            m_last_read = instances.back() + m_end.size();
            m_stream.seekg(back, std::ios_base::cur);
        } else {
            // check if any chars of m_end are present at the end of the buffer
            std::string endstr = m_end;
            char e = m_end.back();

            size_t offset = m_buffer_size - 1;
            bool is_partial_match = true;
            while(endstr.size()) {
                if(m_buffer[offset] == e) {
                    e = endstr.back();
                    offset--;
                } else if(offset != m_buffer_size - 1) {
                    is_partial_match = false;
                    break;
                } else {
                    e = endstr.back();
                }
                endstr.pop_back();
            }

            if(is_partial_match) {
                int64_t back = m_buffer_size - offset - 1;
                m_last_read = m_buffer_size - back;
                m_stream.seekg(-back, std::ios_base::cur);
            }
        }

        if(back) {
            m_last_read -= back;
        }
        return true;
    }


    void AsciiStreamReader::NewFile(const std::string &_file_name) {
        // check if stream was previously opened, and if it was, close it
        CloseFile();

        m_buffer = new char[m_buffer_size]{};
        m_stream.open(_file_name, std::ios::binary);

        if(!m_stream.is_open() || !m_stream.good()) {
            std::cerr << "Failed to open file " << _file_name << std::endl;
            std::exit(1);
        }

        // determine the file size
        m_stream_size = m_stream.tellg();
        m_stream.seekg(0, std::ios::end);
        m_stream_size = static_cast<size_t>(m_stream.tellg()) - m_stream_size;
        m_stream.seekg(0, std::ios::beg);
    }


    void AsciiStreamReader::CloseFile() {
        if(m_stream.is_open()) {
            m_stream.close();
            delete [] m_buffer;
            m_buffer = nullptr;
        }
    }


    char *AsciiStreamReader::GetBufferAddress() {
        return m_buffer;
    }


    std::string AsciiStreamReader::GetFileName() {
        return m_file_name;
    }
}