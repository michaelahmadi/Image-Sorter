#include "ImageSorter.hpp"


int main(int argc, char **argv)
{
    
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file\n";
        return 1;
    }
    
    boost::filesystem::path p(argv[1]);
    
    if(exists(p))
    {

        if(is_regular_file(p))
        {
            std::cout << p << " size is " << file_size(p) << std::endl;
        }
        else if(is_directory(p))
        {
            std::cout << p << " is a directory" << std::endl;
        }
        else
            std::cout << p << " is neither file nor directory" << std::endl;
        
    }
    else
        std::cout << p << " doesnt exist" << std::endl;

    
    std::cout << "Hello world!" << std::endl;
    return 0;
}