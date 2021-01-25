# Image-Sorter
An command line application in C++ that sorts images into folders by year and month.

Third Party Libraries:
Boost Filesystem, Exiv2

    sudo apt-get install libboost-filesystem-dev
    sudo apt-get install libboost-system-dev
    sudo apt-get install libexiv2-dev

To Use:

    make
    ./ImageSorter [dir]

Make sure [dir] contains only .JPG images. For best results, make sure all images have exif metadata.

After running, a folder named "[dir] Sorting Folder" will be created in the same directory as [dir].
Inside that folder will be folders by year and month to which the images in [dir] will be copied.