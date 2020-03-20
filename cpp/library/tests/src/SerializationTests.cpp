#include <boost/test/unit_test.hpp>

#include "library/Book.hpp"
#include "library/Borrower.hpp"
#include "library/Employee.hpp"
#include "library/Library.hpp"
#include "library/LibraryFactory.hpp"
#include "library/LibraryPackage.hpp"
#include "library/Writer.hpp"

#include "ecore/EResource.hpp"
#include "ecore/EResourceFactory.hpp"
#include "ecore/EResourceFactoryRegistry.hpp"
#include "ecore/URI.hpp"

#include <chrono>
#include <random>
#include <stack>
#include <fstream>

using namespace ecore;
using namespace library;

namespace
{
    constexpr int nb_employees = 10;
    constexpr int nb_writers = 100;
    constexpr int nb_books = 1000;
    constexpr int nb_borrowers = 100;

    template <class TimePoint>
    class uniform_time_distribution
    {
    public:
        uniform_time_distribution( TimePoint start, TimePoint end )
            : m_start( start )
            , m_end( end )
            , m_seconds( std::chrono::duration_cast<std::chrono::seconds>( end - start ) )
        {
        }

        template <class Generator>
        TimePoint operator()( Generator&& g )
        {
            std::uniform_int_distribution<std::chrono::seconds::rep> d( 0, m_seconds.count() );

            return m_start + std::chrono::seconds( d( g ) );
        }

    private:
        TimePoint m_start;
        TimePoint m_end;
        std::chrono::seconds m_seconds;
    };

    template <class TimePoint>
    TimePoint randomTime( TimePoint start, TimePoint end )
    {
        static std::random_device rd;
        static std::mt19937 gen( rd() );
        uniform_time_distribution<TimePoint> t( start, end );
        return t( gen );
    }

} // namespace

BOOST_AUTO_TEST_SUITE( SerializationTests )

BOOST_AUTO_TEST_CASE( GenerateModel, *boost::unit_test::disabled() )
{
    auto lf = LibraryFactory::eInstance();
    auto lp = LibraryPackage::eInstance();

    auto l = lf->createLibrary();
    l->setName( "My Library" );
    l->setAddress( "My Library Adress" );

    std::default_random_engine generator;

    // employees
    auto employees = l->getEmployees();
    for( int i = 0; i < nb_employees; ++i )
    {
        auto ndx = std::to_string( i );
        auto e = lf->createEmployee();
        e->setAddress( "Adress " + ndx );
        e->setFirstName( "First Name " + ndx );
        e->setLastName( "Last Name " + ndx );

        if( !employees->empty() )
        {
            std::uniform_int_distribution<int> d( 0, static_cast<int>( employees->size() ) - 1 );
            auto ndx = d( generator );
            e->setManager( employees->get( ndx ) );
        }

        employees->add( e );
    }

    // writers
    for( int i = 0; i < nb_writers; ++i )
    {
        auto w = lf->createWriter();
        auto ndx = std::to_string( i );
        w->setAddress( "Adress " + ndx );
        w->setFirstName( "First Name " + ndx );
        w->setLastName( "Last Name " + ndx );

        l->getWriters()->add( w );
    }

    // books
    std::uniform_int_distribution<int> book_author_distibution( 0, nb_writers > 0 ? nb_writers - 1 : 0 );
    std::uniform_int_distribution<int> book_category_distibution( 0, 2 );
    std::uniform_int_distribution<int> book_copies_distibution( 1, 5 );
    std::uniform_int_distribution<int> book_pages_distibution( 1, 500 );
    auto start_date = std::chrono::system_clock::from_time_t( std::time_t( 0 ) );
    auto end_date = std::chrono::system_clock::now();
    for( int i = 0; i < nb_books; ++i )
    {
        auto b = lf->createBook();
        b->setCategory( BookCategory( book_category_distibution( generator ) ) );
        b->setCopies( book_copies_distibution( generator ) );
        b->setPages( book_pages_distibution( generator ) );
        b->setPublicationDate( std::chrono::system_clock::to_time_t( randomTime( start_date, end_date ) ) );
        b->setTitle( "Title " + std::to_string( i ) );

        if constexpr( nb_writers > 0 )
        {
            auto authorNdx = book_author_distibution( generator );
            auto a = l->getWriters()->get( authorNdx );
            b->setAuthor( a );
        }
        
        l->getBooks()->add( b );
    }

    // borrowers
    std::uniform_int_distribution<int> borrower_book_distibution( 0, nb_borrowers > 0 ? nb_borrowers - 1 : 0 );
    for( int i = 0; i < nb_borrowers; ++i )
    {
        auto b = lf->createBorrower();
        auto ndx = std::to_string( i );
        b->setAddress( "Adress " + ndx );
        b->setFirstName( "First Name " + ndx );
        b->setLastName( "Last Name " + ndx );

        if constexpr ( nb_books > 0 )
        {
            auto bookNdx = borrower_book_distibution( generator );
            auto book = l->getBooks()->get( bookNdx );
            b->getBorrowed()->add( book );
        }
    }

    auto fileURI = URI("file:D:/dev/mylib.xml");
    auto resourceFactory = EResourceFactoryRegistry::getInstance()->getFactory( fileURI );
    BOOST_CHECK( resourceFactory );
    auto resource = resourceFactory->createResource( fileURI );
    BOOST_CHECK( resource );
    resource->getContents()->add( l );
    resource->save();
}

namespace
{
    std::string replaceAll( std::string str, const std::string& from, const std::string& to )
    {
        size_t start_pos = 0;
        while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
        {
            str.replace( start_pos, from.length(), to );
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
} // namespace

BOOST_AUTO_TEST_CASE( LoadSave )
{
    auto fileURI = URI( "data/library.xml" );
    auto resourceFactory = EResourceFactoryRegistry::getInstance()->getFactory( fileURI );
    BOOST_CHECK( resourceFactory );
    auto resource = resourceFactory->createResource( fileURI );
    BOOST_CHECK( resource );
    resource->load();

    BOOST_CHECK( resource->isLoaded() );
    BOOST_CHECK( resource->getWarnings()->empty() );
    BOOST_CHECK( resource->getErrors()->empty() );

    std::ifstream ifs( "data/library.ecore" );
    std::string expected( ( std::istreambuf_iterator<char>( ifs ) ), std::istreambuf_iterator<char>() );

    std::stringstream ss;
    resource->save( ss );

    BOOST_CHECK_EQUAL( replaceAll( ss.str(), "\r\n", "\n" ), replaceAll( expected, "\r\n", "\n" ) );
}

BOOST_AUTO_TEST_SUITE_END()