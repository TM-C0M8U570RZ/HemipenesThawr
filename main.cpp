#include <iostream>
#include <fstream>
#include <tmc_application.h>

void printHelp();

u32 bytesToU32LE(std::vector<u8>& data);

u64 bytesToU64LE(std::vector<u8>& data);

int main(int argc, char** argv)
{
    tmc::Application* a = tmc::Application::getGlobalInstance(argc,
                                                              argv,
                                                              std::filesystem::path(),
                                                              std::filesystem::path(),
                                                              "org.TMC0M8U570RZ.TomIsACoolArtist");
    if (a->hasArg("honoo") || a->hasArg("flareon") || a->hasArg("booster") || a->hasArg("buusutaa"))
    {
        std::cout << "To Tom Smith:\n"
                     "I'm sorry, lo siento and sumimasen for whatever it was\n"
                     "that caused you to block me on both X and Bluesky.\n"
                     "I'm inspired by your furry art as I myself am learning\n"
                     "to be a furry artist.  Sure my furry art is mostly what\n"
                     "you were banned from drawing five years ago, but I still\n"
                     "think that you hold a special place in my heart.\n"
                     "I think it would be really awesome if you could unblock\n"
                     "me and help me improve my furry art even if it means\n"
                     "associating with someone who draws what you were banned\n"
                     "from drawing.  I made this tool as I personally see\n"
                     "modding games as a form of appreciation for the devs of\n"
                     "said games.  You should check out my mod of Pokemon\n"
                     "Scarlet and Violet that adds genitals to Vaporeon's\n"
                     "model.  I know from what happened to people like you,\n"
                     "Gary Bowser and Bunnei not to profit off Nintendo IP\n"
                     "That's why I will not be creating a Patreon for my\n"
                     "furry art like you did.  I don't want to end up as\n"
                     "a Category 8 Nintendo Ninja victim like you.\n"
                     "I will still be carrying out my ~14-year plan to get\n"
                     "DMCA abolished that involves becoming president and\n"
                     "withdrawing from the UN.  Some details of that plan\n"
                     "are still up in the air tho such as: will I become\n"
                     "president legitimately through votes by spreading\n"
                     "anti-DMCA propaganda or will I become president by\n"
                     "genetically engineering real living Pokemon with\n"
                     "minds intelligent enough to seek out humans\n"
                     "corrupting the government and eat them?\n"
                     "Hopefully the anti-DMCA propaganda works so my\n"
                     "real living Pokemon don't have to be even more\n"
                     "unethical than they already are.\n"
                     "I look forward to Panic in Puzzle World's release\n"
                     "And I hope you do the same with Honoo no Akai Roketto.\n"
                     "May Honoo the Flareon protect you.\n\n";
    }
    if (a->hasArg('h') || a->hasArg("help"))
    {
        printHelp();
        return 1;
    }
    if (!(a->hasArg('i') && a->hasArg('o')))
    {
        printHelp();
        std::cerr << "\nERROR!  Both -i and -o must be specified!\n";
        return -1;
    }
    std::filesystem::path infile = a->getAs<std::filesystem::path>('i');
    if (!std::filesystem::is_regular_file(infile))
    {
        printHelp();
        std::cerr << "\nERROR!  -i must be an existing file!\n";
        return -2;
    }
    std::filesystem::path outdir = a->getAs<std::filesystem::path>('o');
    if (std::filesystem::exists(outdir) && !std::filesystem::is_directory(outdir))
    {
        printHelp();
        std::cerr << "\nERROR!  -o must either be nonexistant or a directory!\n";
        return -2;
    }
    if (!std::filesystem::exists(outdir))
    {
        std::cout << "Creating output directory\n";
        if (!std::filesystem::create_directories(outdir))
        {
            std::cerr << "ERROR!  Access denied!\n";
            return -2;
        }
    }
    std::ifstream in(infile, std::ios::binary);
    if (in.is_open())
    {
        std::vector<u8> magic(4, 0);
        in.read(reinterpret_cast<char*>(magic.data()), magic.size());
        for (u8 i = 0; i < 4; i++)
        {
            if (magic[i] != 0)
            {
                std::cerr << "ERROR!  The file data is destroyed!\n";
                return -3;
            }
        }
        magic.clear();
        std::vector<u8> sizeReader(4, 0);
        in.read(reinterpret_cast<char*>(sizeReader.data()), sizeReader.size());
        u32 entryCount = bytesToU32LE(sizeReader);
        std::cout << "Found " << entryCount << " files.\n\n";
        sizeReader.clear();
        for (u32 i = 0; i < entryCount; i++)
        {
            sizeReader.push_back(in.get());
            in.seekg(3, std::ios::cur);
            u8 folderDepth = sizeReader[0];
            std::filesystem::path current = outdir;
            std::vector<u8> dataReader;
            for (u8 j = 0; j < folderDepth - 1; j++)
            {
                sizeReader[0] = in.get();
                dataReader = std::vector<u8>(sizeReader[0], 0);
                in.read(reinterpret_cast<char*>(dataReader.data()), dataReader.size());
                std::string filename;
                for (u8 k = 0; k < dataReader.size(); k++)
                {
                    filename.push_back(static_cast<char>(dataReader[k]));
                }
                current /= std::filesystem::path(filename);
                if (!std::filesystem::exists(current))
                {
                    std::cout << "Saving " << current << "\n";
                    if (!std::filesystem::create_directories(current))
                    {
                        std::cerr << "ERROR!  Access denied!\n";
                        return -2;
                    }
                }
            }
            sizeReader[0] = in.get();
            dataReader = std::vector<u8>(sizeReader[0], 0);
            in.read(reinterpret_cast<char*>(dataReader.data()), dataReader.size());
            std::string filename;
            for (u8 j = 0; j < dataReader.size(); j++)
            {
                filename.push_back(static_cast<char>(dataReader[j]));
            }
            current /= std::filesystem::path(filename);
            if (!std::filesystem::exists(current))
            {
                std::cout << "Saving " << current << "\n";
                std::ofstream out(current, std::ios::binary);
                if (!out.is_open())
                {
                    std::cerr << "ERROR!  Access denied or out of space!\n";
                    return -4;
                }
                else
                {
                    sizeReader.clear();
                    sizeReader.resize(8, 0);
                    in.read(reinterpret_cast<char*>(sizeReader.data()), sizeReader.size());
                    u64 fileSize = bytesToU64LE(sizeReader);
                    dataReader.clear();
                    dataReader.resize(fileSize, 0);
                    in.read(reinterpret_cast<char*>(dataReader.data()), dataReader.size());
                    out.write(reinterpret_cast<char*>(dataReader.data()), dataReader.size());
                    out.close();
                    sizeReader.clear();
                }
            }
        }
        in.close();
    }
    else
    {
        std::cerr << "ERROR!  Access denied!\n";
        return -2;
    }
    return 0;
}

void printHelp()
{
    std::cout << "Usage:\n"
              << "\tHemipenesThawr -i <input.dat> -o <outputDirectory>\n";
}

// One little two little three little endians
// Four little five little six little endians
// Seven little eight little nine little endians
// Ten little endian bytes (even tho this function is for four of them)
u32 bytesToU32LE(std::vector<u8>& data)
{
    if (data.size() < 4)
    {
        throw std::invalid_argument("Too smol");
    }
    u32 result = (static_cast<u32>(data[0]) << 0) |
                 (static_cast<u32>(data[1]) << 8) |
                 (static_cast<u32>(data[2]) << 16) |
                 (static_cast<u32>(data[3]) << 24);
    return result;
}


u64 bytesToU64LE(std::vector<u8>& data)
{
    if (data.size() < 8)
    {
        throw std::invalid_argument("Too smol");
    }
    u64 result = (static_cast<u64>(data[0]) << 0) |
                 (static_cast<u64>(data[1]) << 8) |
                 (static_cast<u64>(data[2]) << 16) |
                 (static_cast<u64>(data[3]) << 24) |
                 (static_cast<u64>(data[4]) << 32) |
                 (static_cast<u64>(data[5]) << 40) |
                 (static_cast<u64>(data[6]) << 48) |
                 (static_cast<u64>(data[7]) << 56);
    return result;
}
