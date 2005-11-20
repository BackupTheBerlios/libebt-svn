/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <libebt/libebt.hh>
#include <libebt/libebt_util.hh>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <cstdlib>

/* Tag for our exception context. */
struct ExceptionTag { };

/* Exception class. */
class Exception : public libebt::Backtracable<ExceptionTag>,
                  public std::exception
{
    protected:
        const std::string _message;

    public:
        Exception(const std::string & message) throw () :
            libebt::Backtracable<ExceptionTag>(),
            std::exception(),
            _message(message)
        {
        }

        virtual ~Exception() throw ()
        {
        }

        std::string message() const throw ()
        {
            return _message;
        }
};

/* Thrown if we can't open a file. */
class CannotOpenFileException : public Exception
{
    public:
        CannotOpenFileException(const std::string & filename) throw () :
            Exception("Cannot open file '" + filename + "'")
        {
        }
};

/* Thrown if we encounter a '#e' directive in a file. */
class FileHasErrorException : public Exception
{
    public:
        FileHasErrorException(unsigned line_number, const std::string & line) throw () :
            Exception("Error directive on line " + libebt::stringify(line_number) +
                    ": '" + line + "'")
        {
        }
};

/* Thrown if we encounter a bad # line in a file. */
class UnknownDirectiveException : public Exception
{
    public:
        UnknownDirectiveException(unsigned line_number, const std::string & line) throw () :
            Exception("Unknown directive on line " + libebt::stringify(line_number) +
                    ": '" + line + "'")
        {
        }
};

/* Thrown if we include too deeply (for example, recursive includes). */
class IncludeDepthTooDeepException : public Exception
{
    public:
        IncludeDepthTooDeepException() throw () :
            Exception("Include depth too deep")
        {
        }
};

/* Convenience definition for our backtrace context. */
typedef libebt::BacktraceContext<ExceptionTag, std::string> Context;

/* Process a file. Handle directives, and echo everything else to stdout. */
void
process_file(const std::string & filename, unsigned depth = 0)
{
    Context c("In file '" + filename + "'");

    if (depth > 10)
        throw IncludeDepthTooDeepException();

    std::ifstream file(filename.c_str());
    if (! file)
        throw CannotOpenFileException(filename);

    std::string line;
    unsigned line_number = 0;
    while (std::getline(file, line))
    {
        ++line_number;

        if (0 == line.compare(0, 3, "#e "))
            throw FileHasErrorException(line_number, line.substr(3));
        else if (0 == line.compare(0, 3, "#i "))
        {
            Context c2("From #i directive on line " + libebt::stringify(line_number));
            process_file(line.substr(3), depth + 1);
        }
        else if (0 == line.compare(0, 1, "#"))
            throw UnknownDirectiveException(line_number, line);
        else
            std::cout << line << std::endl;
    }
}

int
main(int argc, char *argv[])
{
    Context c("In main program");

    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
            return EXIT_FAILURE;
        }

        for (int argi = 1 ; argi < argc ; ++argi)
        {
            Context c2("When handling commandline argument '" +
                    libebt::stringify(argv[argi]) + "'");
            process_file(argv[argi]);
        }

        return EXIT_SUCCESS;
    }
    catch (Exception & e)
    {
        std::cerr << "Error!\n  * " << e.backtrace(":\n  * ")
            << e.message() << " (" << e.what() << ")" << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!" << std::endl;
        return EXIT_FAILURE;
    }
}

