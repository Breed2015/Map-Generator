#include <iostream>
#include <sstream>

#include "Tools.h"
#include "Map.h"

#include "IslandGenerator.h"
#include "MapOutput.h"

using namespace std;

void displayHelp()
{
	cout <<
		"  --help, -H\n"
		"            Displays available parameters\n"
		"  --size, -S (REQUIRED)\n"
		"            Arguments: size\n"
		"            Sets generated map size\n"
		"  --filename, -F (REQUIRED?)\n"
		"            Name of file with the map, don't add extension!\n"
		"            Required if there's no --prefix argument present\n"
		"  --prefix, -P (REQUIRED?)\n"
		"            Prefix for generated files, don't add extension!\n"
		"            Required if there's no --filename argument present\n"
		"  --count, -C\n"
		"            Default value: 1\n"
		"            Count of files to be generated\n"
		"  --format\n"
		"            Available formats: PNG|JSON|LUA\n"
		"            Default value: PNG\n"
		"            Format of output file\n"
		"  --preset, -P\n"
		"            Available presets: island\n"
		"            Default value: island\n"
		"            Preset for generated map";
}

int main(int argCount, char *argv[])
{
	if (argCount == 0)
		return 0;

	// Parsing parameters
	vector<string> availableFormats {"json", "png", "lua"};
	vector<string> availablePresets {"island"};

	string arg = "", prefix = "", filename = "", format = "png", preset = "island";
	int size = 0, count = 1;

	for (int i = 0; i < argCount; i++)
	{
		arg = argv[i];
		cout << arg << endl;

		if (arg == "--help" || arg == "-H")
		{
			displayHelp();
			return 0;
		}
		else if (arg == "-S" || arg == "--size")
		{
			i++;
			size = atoi(argv[i]);
		}
		else if (arg == "-F" || arg == "--filename")
		{
			i++;
			arg = argv[i];
			filename = arg;
		}
		else if (arg == "-P" || arg == "--prefix")
		{
			i++;
			arg = argv[i];
			prefix = arg;
		}
		else if (arg == "-C" || arg == "--count")
		{
			i++;
			arg = argv[i];
			count = atoi(arg.c_str());
		}
		else if (arg == "--format")
		{
			i++;
			arg = argv[i];
			toLowerCase(arg);
			format = arg;

			bool found = false;
			for (auto _format : availableFormats)
			{
				if (_format == format)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				cout << "Error: Unknown format, use --help to get more info.";
				return 1;
			}
		}
		else if (arg == "-P" || arg == "--preset")
		{
			i++;
			arg = argv[i];
			toLowerCase(arg);
			preset = arg;

			bool found = false;
			for (auto _preset : availablePresets)
			{
				if (_preset == preset)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				cout << "Error: Unknown preset, use --help to get more info.";
				return 1;
			}
		}
	}

	// Validating
	if (prefix != "" && filename != "")
	{
		cout << "Error: You can only set --prefix OR --filename. Choose wisely.";
		return 1;
	}

	if (filename != "" && count > 1)
	{
		cout << "Error: --count argument doesn't work with --filename argument. You should try --prefix argument instead.";
		return 1;
	}

	if (size == 0)
	{
		cout << "Error: Could not generate map, use -S to set its size.";
		return 1;
	}

	Map map(size);

	// Generating & saving map
	string saveName;
	for (int i = 0; i < count; i++)
	{
		// Generating
		map.clearMap();

		if (preset == "island")
		{
			IslandGenerator::generate(map);
		}

		// Saving
		if (count == 1)
			saveName = filename;
		else
		{
			stringstream ss;
			ss << prefix << i;
			saveName = ss.str();
		}

		if (format == "png")
			MapOutput::saveAsPNG(map, saveName + ".png");
		else if (format == "lua")
			MapOutput::saveAsLUA(map, saveName + ".lua");
		else if (format == "json")
			MapOutput::saveAsJSON(map, saveName + ".json");
	}

	return 0;
}