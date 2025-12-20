#include <iostream>
#include <vector>
#include <string>
#include <filesystem> // Required for file system operations
#include <fstream>
#include <set>
#include <algorithm>
namespace fs = std::filesystem; // Shorter alias for std::filesystem

std::vector<fs::path> findJPGFiles(const fs::path &rootPath)
{
    std::vector<fs::path> jpgFiles;

    if (!fs::exists(rootPath) || !fs::is_directory(rootPath))
    {
        std::cerr << "Error: Invalid root path or not a directory." << std::endl;
        return jpgFiles;
    }

    for (const auto &entry : fs::recursive_directory_iterator(rootPath))
    {
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".jpg")
        {
            jpgFiles.push_back(entry.path());
        }
    }
    return jpgFiles;
}

void divideIntoBatches(const std::vector<fs::path> &filePaths, size_t batchSize, std::vector<std::vector<fs::path>> &batches)
{
    size_t totalFiles = filePaths.size();
    for (size_t i = 0; i < totalFiles; i += batchSize)
    {
        size_t end = std::min(i + batchSize, totalFiles);
        batches.emplace_back(filePaths.begin() + i, filePaths.begin() + end);
    }
}
// int main()
// {
//     fs::path rootFolder = "F:\\Recipe1M\\test";          // Replace with your desired root folder
//     fs::path destFolder = "F:\\Recipe1M\\filtered_test"; // Destination folder for copied .jpg files
//     std::vector<fs::path> foundJPGFiles = findJPGFiles(rootFolder);

//     if (foundJPGFiles.empty())
//     {
//         std::cout << "No .jpg files found in " << rootFolder << " and its subfolders." << std::endl;
//     }
//     // else
//     // {
//     //     std::ofstream outFile("jpg_files.txt");
//     //     if (!outFile)
//     //     {
//     //         std::cerr << "Error: Could not open output file." << std::endl;
//     //         return 1;
//     //     }
//     //     for (const auto &filePath : foundJPGFiles)
//     //     {
//     //         outFile << filePath.filename().string() << std::endl;
//     //     }
//     //     outFile.close();
//     // }
//     // else
//     // {
//     //     for (const auto &filePath : foundJPGFiles)
//     //     {
//     //         fs::path destPath = destFolder / filePath.filename();
//     //         try
//     //         {
//     //             fs::copy_file(filePath, destPath, fs::copy_options::overwrite_existing);
//     //         }
//     //         catch (fs::filesystem_error &e)
//     //         {
//     //             std::cerr << "Error copying " << filePath << ": " << e.what() << std::endl;
//     //         }
//     //     }
//     //     std::cout << "Copied " << foundJPGFiles.size() << " .jpg files to " << destFolder << std::endl;
//     // }
//     // fs::path rootFolder = "F:\\Recipe1M\\filtered_test";   // Source folder
//     // fs::path destFolder = "F:\\Recipe1M\\filtered_test_2"; // Destination folder

//     // Read filtered image names into a set
//     std::set<std::string> filteredImageNames;
//     std::ifstream filterFile("filtered_image_names.txt");
//     std::string line;
//     while (std::getline(filterFile, line))
//     {
//         if (!line.empty())
//         {
//             if (line.front() == '"' && line.back() == '"')
//                 line = line.substr(1, line.size() - 2); // strip quotes
//             filteredImageNames.insert(line);
//             std::cout << line << std::endl;
//         }
//     }
//     filterFile.close();

//     // Create destination folder if it doesn't exist
//     if (!fs::exists(destFolder))
//     {
//         fs::create_directories(destFolder);
//     }

//     // Traverse and process jpg files
//     for (const auto &entry : fs::recursive_directory_iterator(rootFolder))
//     {
//         if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".jpg")
//         {
//             std::string filename = entry.path().filename().string();
//             if (filteredImageNames.count(filename))
//             {
//                 // Copy to destination folder
//                 fs::path destPath = destFolder / filename;
//                 try
//                 {
//                     fs::copy_file(entry.path(), destPath, fs::copy_options::overwrite_existing);
//                 }
//                 catch (fs::filesystem_error &e)
//                 {
//                     std::cerr << "Error copying " << entry.path() << ": " << e.what() << std::endl;
//                 }
//             }
//         }
//     }
//     std::cout << "Copying complete." << std::endl;
//     // std::cout << "Processing complete." << std::endl;
//     return 0;
// }

void folderToZip(const fs::path &folderPath, const fs::path &zipPath)
{
    // Placeholder function: Implement zip functionality using a suitable library
    std::cout << "Zipping folder " << folderPath << " to " << zipPath << std::endl;
}
#include <sstream>
// ...existing includes and functions...

int main()
{
    fs::path rootFolder = "F:\\Recipe1M\\filtered_test"; // Folder containing all images to split
    size_t batchSize = 537;

    // Find all jpg files in the folder (not recursive, just this folder)
    std::vector<fs::path> imageFiles;
    for (const auto &entry : fs::directory_iterator(rootFolder))
    {
        if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".jpg")
        {
            imageFiles.push_back(entry.path());
        }
    }

    // Sort files for consistent ordering
    std::sort(imageFiles.begin(), imageFiles.end());

    // Divide into batches
    std::vector<std::vector<fs::path>> batches;
    divideIntoBatches(imageFiles, batchSize, batches);

    // Create subfolders and move images
    size_t imgIndex = 1;
    for (size_t i = 0; i < batches.size(); ++i)
    {
        size_t startIdx = imgIndex;
        size_t endIdx = imgIndex + batches[i].size() - 1;
        std::ostringstream subfolderName;
        subfolderName << "Image" << startIdx << "to" << endIdx;
        fs::path subfolder = rootFolder / subfolderName.str();
        if (!fs::exists(subfolder))
            fs::create_directory(subfolder);

        for (const auto &imgPath : batches[i])
        {
            fs::path destPath = subfolder / imgPath.filename();
            try
            {
                fs::rename(imgPath, destPath); // Move file
            }
            catch (fs::filesystem_error &e)
            {
                std::cerr << "Error moving " << imgPath << ": " << e.what() << std::endl;
            }
        }
        imgIndex += batches[i].size();
    }

    std::cout << "Divided " << imageFiles.size() << " images into " << batches.size() << " subfolders." << std::endl;
    return 0;
}