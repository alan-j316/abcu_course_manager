#include "CSVparser.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

namespace csv
{

    Parser::Parser(const std::string& data, const DataType& type, char sep): _type(type), _sep(sep)
    {
        std::string line;
        if (type == eFILE)
        {
            _file = data;
            std::ifstream ifile(_file.c_str());
            if (ifile.is_open())
            {
                while (ifile.good())
                {
                    getline(ifile, line);
                    if (line != "")
                        _originalFile.push_back(line);
                }
                ifile.close();

                if (_originalFile.size() == 0)
                    throw Error(std::string("No Data in ").append(_file));

                parseHeader();
                parseContent();
            }
            else
                throw Error(std::string("Failed to open ").append(_file));
        }
        else
        {
            std::istringstream stream(data);
            while (std::getline(stream, line))
                if (line != "")
                    _originalFile.push_back(line);
            if (_originalFile.size() == 0)
                throw Error(std::string("No Data in pure content"));

            parseHeader();
            parseContent();
        }
    }

    Parser::~Parser()
    {

        for (auto it = _content.begin(); it != _content.end(); ++it)
            delete *it;
    }

    void Parser::parseHeader()
    {
        std::stringstream ss(_originalFile[0]);
        std::string item;

        while (std::getline(ss, item, _sep))
            _header.push_back(item);
    }

    void Parser::parseContent()
    {

        auto it = _originalFile.begin();
        ++it; // skip header

        for (; it != _originalFile.end(); ++it)
        {
            bool quoted = false;
            int tokenStart = 0;

            auto row = new Row(_header);

            for (unsigned int i = 0; i != it->length(); i++)
            {
                if (it->at(i) == '"')
                    quoted = !quoted;
                else if (it->at(i) == ',' && !quoted)
                {
                    row->push(it->substr(tokenStart, i - tokenStart));
                    tokenStart = static_cast<int>(i) + 1;
                }
            }

            // end
            row->push(it->substr(tokenStart, it->length() - tokenStart));

            // if value(s) missing
            if (row->size() != _header.size())
                throw Error("corrupted data !");
            _content.push_back(row);
            delete row;
        }
    }

    Row& Parser::getRow(const unsigned int row) const
    {
        if (row < _content.size())
            return *_content[row];
        throw Error("can't return this row (doesn't exist)");
    }

    Row& Parser::operator[](const unsigned int row) const
    {
        return getRow(row);
    }

    unsigned int Parser::rowCount() const
    {
        return _content.size();
    }

    unsigned int Parser::columnCount() const
    {
        return _header.size();
    }

    std::vector<std::string> Parser::getHeader() const
    {
        return _header;
    }

    std::string Parser::getHeaderElement(unsigned int pos) const
    {
        if (pos >= _header.size())
            throw Error("can't return this header (doesn't exist)");
        return _header[pos];
    }

    bool Parser::deleteRow(unsigned int pos)
    {
        if (pos < _content.size())
        {
            delete *(_content.begin() + pos);
            _content.erase(_content.begin() + pos);
            return true;
        }
        return false;
    }

    bool Parser::addRow(const unsigned int pos, const std::vector<std::string>& r)
    {
        const auto row = new Row(_header);

        for (auto it = r.begin(); it != r.end(); ++it)
            row->push(*it);

        if (pos <= _content.size())
        {
            _content.insert(_content.begin() + pos, row);
            delete row;
            return true;
        }
        delete row;
        return false;
    }

    void Parser::sync() const
    {
        if (_type == eFILE)
        {
            std::ofstream f;
            f.open(_file, std::ios::out | std::ios::trunc);

            // header
            unsigned int i = 0;
            for (auto it = _header.begin(); it != _header.end(); ++it)
            {
                f << *it;
                if (i < _header.size() - 1)
                    f << ",";
                else
                    f << std::endl;
                i++;
            }

            for (auto it = _content.begin(); it != _content.end(); ++it)
                f << **it << std::endl;
            f.close();
        }
    }

    const std::string& Parser::getFileName() const
    {
        return _file;
    }

    /*
    ** ROW
    */

    Row::Row(const std::vector<std::string>& header): _header(header) {}

    Row::~Row() {}

    unsigned int Row::size() const
    {
        return _values.size();
    }

    void Row::push(const std::string& value)
    {
        _values.push_back(value);
    }

    bool Row::set(const std::string& key, const std::string& value)
    {
        int pos = 0;

        for (auto it = _header.begin(); it != _header.end(); ++it)
        {
            if (key == *it)
            {
                _values[pos] = value;
                return true;
            }
            pos++;
        }
        return false;
    }

    std::string Row::operator[](const unsigned int valuePosition) const
    {
        if (valuePosition < _values.size())
            return _values[valuePosition];
        throw Error("can't return this value (doesn't exist)");
    }

    std::string Row::operator[](const std::string& valueName) const
    {
        int pos = 0;

        for (auto it = _header.begin(); it != _header.end(); ++it)
        {
            if (valueName == *it)
                return _values[pos];
            pos++;
        }

        throw Error("can't return this value (doesn't exist)");
    }

    std::ostream& operator<<(std::ostream& os, const Row& row)
    {
        for (const auto& _value : row._values)
            os << _value << " | ";

        return os;
    }

    std::ofstream& operator<<(std::ofstream& os, const Row& row)
    {
        for (unsigned int i = 0; i != row._values.size(); i++)
        {
            os << row._values[i];
            if (i < row._values.size() - 1)
                os << ",";
        }
        return os;
    }
} // namespace csv
