#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include <algorithm>
#include <set>
using json = nlohmann::json;
using namespace std;

// ...existing code...

string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

vector<string> read_image_names(const string &filename)
{
    vector<string> image_names;
    ifstream infile(filename);
    string name;
    while (getline(infile, name))
    {
        name = trim(name);
        if (!name.empty())
            image_names.push_back(name);
    }
    return image_names;
}

vector<string> get_dish_ids_by_image_names(const string &json_filename, const vector<string> &image_names)
{
    set<string> result;
    ifstream infile(json_filename);
    string json_content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    try
    {
        auto j_array = json::parse(json_content);
        for (const auto &j : j_array)
        {
            string dish_id = j["id"];
            for (const auto &img : j["images"])
            {
                string img_id = img["id"];
                for (const auto &name : image_names)
                {
                    if (img_id == name)
                    {
                        cout << "Found matching dish id: " << dish_id << " for image: " << img_id << endl;
                        result.insert(dish_id);
                        break;
                    }
                }
            }
        }
    }
    catch (json::parse_error &e)
    {
        cout << "JSON parse error: " << e.what() << endl;
    }
    return vector<string>(result.begin(), result.end());
}

void write_dish_ids(const string &filename, const vector<string> &dish_ids)
{
    ofstream outfile(filename);
    for (const auto &id : dish_ids)
    {
        outfile << id << endl;
    }
}

void write_dish_details(const string &json_filename, const vector<string> &dish_ids, const string &output_filename)
{
    ofstream outfile(output_filename);
    if (!outfile.is_open())
    {
        cout << "Failed to open output file." << endl;
        return;
    }
    set<string> id_set(dish_ids.begin(), dish_ids.end());
    ifstream infile(json_filename);
    string json_content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    try
    {
        auto j_array = json::parse(json_content);
        for (const auto &j : j_array)
        {
            string dish_id = j["id"];
            if (id_set.count(dish_id))
            {
                cout << "Writing dish: " << dish_id << endl;
                outfile << "Dish ID: " << dish_id << endl;
                outfile << "Title: " << j["title"] << endl;

                outfile << "Ingredients:" << endl;
                for (const auto &ing : j["ingredients"])
                {
                    outfile << "  - " << ing["text"] << endl;
                }

                outfile << "Instructions:" << endl;
                for (const auto &inst : j["instructions"])
                {
                    outfile << "  - " << inst["text"] << endl;
                }
                outfile << endl;
            }
        }
    }
    catch (json::parse_error &e)
    {
        cout << "JSON parse error: " << e.what() << endl;
    }
}

void removeIDsWith1ingredientOr1Instruction(const string &json_filename, const vector<string> &dish_ids, const string &output_filename)
{
    ofstream outfile(output_filename);
    if (!outfile.is_open())
    {
        cout << "Failed to open output file." << endl;
        return;
    }
    set<string> id_set(dish_ids.begin(), dish_ids.end());
    ifstream infile(json_filename);
    string json_content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    try
    {
        auto j_array = json::parse(json_content);
        for (const auto &j : j_array)
        {
            string dish_id = j["id"];
            if (id_set.count(dish_id))
            {
                size_t ingredient_count = j["ingredients"].size();
                size_t instruction_count = j["instructions"].size();
                if (ingredient_count > 1 && instruction_count > 1)
                {
                    outfile << dish_id << endl;
                }
            }
        }
    }
    catch (json::parse_error &e)
    {
        cout << "JSON parse error: " << e.what() << endl;
    }
}

void returnImagesWith1ingredientOr1Instruction(const string &json_filename, const vector<string> &dish_ids, const string &output_filename)
{
    ofstream outfile(output_filename);
    if (!outfile.is_open())
    {
        cout << "Failed to open output file." << endl;
        return;
    }
    set<string> id_set(dish_ids.begin(), dish_ids.end());
    ifstream infile(json_filename);
    string json_content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    try
    {
        auto j_array = json::parse(json_content);
        for (const auto &j : j_array)
        {
            string dish_id = j["id"];
            if (id_set.count(dish_id)) // Only for input ids
            {
                if (j.contains("images") && !j["images"].empty())
                {
                    outfile << j["images"][0]["id"] << endl;
                }
            }
        }
    }
    catch (json::parse_error &e)
    {
        cout << "JSON parse error: " << e.what() << endl;
    }
}

int main()
{
    // vector<string> image_names = read_image_names("jpg_files.txt");

    // vector<string> dish_ids = get_dish_ids_by_image_names("layer2.json", image_names);
    // cout << "Total dish ids found: " << dish_ids.size() << endl;
    // write_dish_details("layer1.json", dish_ids, "dish_details.txt");
    // write_dish_ids("test1_ids.txt", dish_ids);

    // vector<string> dish_ids = read_image_names("test1_ids.txt");
    // removeIDsWith1ingredientOr1Instruction("layer1.json", dish_ids, "filtered_dish_ids.txt");
    vector<string> dish_ids = read_image_names("filtered_sorted_idx.txt");
    returnImagesWith1ingredientOr1Instruction("layer2.json", dish_ids, "filtered_image_names_inverse.txt");
    return 0;
}