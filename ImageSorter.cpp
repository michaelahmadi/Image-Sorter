#include "ImageSorter.hpp"

// https://www.exiv2.org/examples.html#example1
// First, complete -> Folder directory + date, check metadata, create new folder with folders based on Dates

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }

    boost::filesystem::path p(argv[1]);

    if (!is_directory(p))
    {
        std::cout << p << " is not a directory" << std::endl;
        return 1;
    }


    
    return 0;
}