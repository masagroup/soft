// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2018 MASA Group
//
// *****************************************************************************

#ifndef ECORE_SAXPARSERPOOL_HPP_
#define ECORE_SAXPARSERPOOL_HPP_

#include "ecore/Exports.hpp"
#include <list>
#include <map>
#include <memory>
#include <string>
#include <xercesc/sax2/SAX2XMLReader.hpp>

namespace ecore::impl
{
    class ECORE_API SaxParserPool
    {
    public:
        class SaxParser
        {
        public:
            SaxParser( SaxParserPool& pool );

            ~SaxParser();

            xercesc::SAX2XMLReader& getReader() const;

            
        private:
            SaxParserPool& pool_;
            std::shared_ptr<xercesc::SAX2XMLReader> reader_;
        };

        static SaxParserPool& getInstance();

        ~SaxParserPool();

        std::unique_ptr<SaxParser> getParser( const std::map<std::string, bool>& features = std::map<std::string, bool>());

    private:
        SaxParserPool();


    private:
        std::list<std::shared_ptr<xercesc::SAX2XMLReader>> readers_;
    };

} // namespace ecore::impl

#endif
