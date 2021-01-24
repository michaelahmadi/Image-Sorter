#include "ImageSorter.hpp"

// https://www.exiv2.org/examples.html#example1
// First, complete -> Folder directory + date, check metadata, create new folder with folders based on Dates

void copyToFolder(const boost::filesystem::path filePath, boost::filesystem::path sortPath, std::string fileDate, int &i)
{
    // Sample Date 2019:08:18 14:34:39
    std::string year = fileDate.substr(0, 4);
    std::string month = fileDate.substr(5, 2);
    
    sortPath += "/" + year;
    // If folder for the year not yet created, create that folder.
    if (!boost::filesystem::exists(sortPath))
    {
        boost::filesystem::create_directory(sortPath);
    }

    sortPath += "/" + month;
    // If folder for the month not yet created, create that folder.
    if (!boost::filesystem::exists(sortPath))
    {
        boost::filesystem::create_directory(sortPath);
        i = 1;
    }

    sortPath += "/" + std::to_string(i) + ".JPG";
    std::cout << "attempting to copy file: " << filePath << " to: " << sortPath << std::endl;

    // Copy file to the sortPath
    try
    {
        boost::filesystem::copy_file(filePath, sortPath);
    }
    catch (std::exception const &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        exit(3);
    }
}

void sortByDate(boost::filesystem::path p)
{
    //create new Folder "Sorting Folder"
    boost::filesystem::path sortingFolder = p;
    sortingFolder += " Sorting Folder"; // This folder will be in the same directory that the target directory is in
    boost::filesystem::create_directory(sortingFolder);

    // iterate though directory p, check metadata of jpegs.
    std::vector<boost::filesystem::path> vec; // store paths,

    std::cout << p << " is a directory containing:\n";

    const char *key = "Exif.Photo.DateTimeOriginal";

    int i = 1; // This will be used to name the copied files
    for (boost::filesystem::directory_entry &x : boost::filesystem::directory_iterator(p))
    {
        // Access metadata
        std::cout << "    " << x.path() << '\n';
        const char *pathName = x.path().c_str();
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(pathName);

        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        if (exifData.empty())
        {
            std::cerr << "no metadata found in file " << x.path() << std::endl;
            exit(2);
        }

        try
        {
            copyToFolder(x.path(), sortingFolder, exifData[key].toString(), i);
            i++;
            //std::cout << exifData[key].toString() << std::endl;
        }
        catch (Exiv2::AnyError &e)
        {
            std::cerr << "Caught Exiv2 exception '" << e << "'" << std::endl;
            exit(3);
        }
        catch (...) // research
        {
            std::cerr << "Caught default exception" << std::endl;
            exit(4);
        }
    }
}

void sortByFace(boost::filesystem::path p)
{
}

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " directory [d|f]\n";
        return 1;
    }

    boost::filesystem::path p(argv[1]);

    if (!is_directory(p))
    {
        std::cout << p << " is not a directory" << std::endl;
        return 1;
    }

    char flag = argv[2][0];
    if (flag == 'd')
    {
        sortByDate(p);
    }
    else if (flag == 'f')
    {
        sortByFace(p);
    }
    else
    {
        std::cout << "Usage: " << argv[0] << " directory [d|f]\n";
        return 1;
    }

    return 0;
}