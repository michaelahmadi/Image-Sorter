#include <string>
#include <iostream>
#include <iterator>
#include <boost/filesystem.hpp>
#include <exiv2/exiv2.hpp>

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

    sortPath += "/" + std::to_string(i) + ".JPG"; // Images are renamed to i.JPG (exif image data is -mostly- .JPG)
    std::cout << "attempting to copy file: " << filePath << " to: " << sortPath << std::endl;

    // Copy file to the sortPath
    try
    {
        boost::filesystem::copy_file(filePath, sortPath, boost::filesystem::copy_option::overwrite_if_exists);
    }
    catch (std::exception const &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        exit(3);
    }
}

void sortByDate(boost::filesystem::path p)
{
    //create new Folder "Sorting Folder"
    boost::filesystem::path sortingFolder = p;
    sortingFolder += " Sorting Folder"; // Destination Directory; In the same directory as the Target Directory
    boost::filesystem::create_directory(sortingFolder);

    // iterate though directory p, check metadata of jpegs.
    const char *key = "Exif.Photo.DateTimeOriginal";
    int i = 1; // This will be used to name the copied files

    for (boost::filesystem::directory_entry &x : boost::filesystem::directory_iterator(p))
    {
        // Access metadata
        const char *pathName = x.path().c_str(); // For handoff between boost::filesystem and Exiv2
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(pathName);
        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        if (!exifData.empty())
        {
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
            catch (...)
            {
                std::cerr << "Caught default exception" << std::endl;
                exit(4);
            }
        }
        else
            std::cout << "no metadata found in file " << x.path() << std::endl;
    }
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [directory]" << std::endl;
        return 1;
    }

    boost::filesystem::path p(argv[1]);

    if (!is_directory(p))
    {
        std::cout << p << " is not a directory" << std::endl;
        return 1;
    }

    sortByDate(p);
    return 0;
}