// FilesSize.cpp : Defines the entry point for the application.
//

#include <string>
#include <iostream>

#include <filesystem>
#include <windows.h>

#define KB 1024
#define MAX_PATH 260

namespace fs = std::filesystem;

std::vector<std::wstring> types_text = { L"pdf", L"doc", L"docs"};
std::vector<std::wstring> types_audio = { L"mp3", L"flac" };
std::vector<std::wstring> types_video = { L"mp4", L"mov" };
std::vector<std::wstring> types_image = { L"png", L"jpg" };

uintmax_t getFileSizeByName(std::wstring name);

int main()
{
	uintmax_t size_video = 0;
	uintmax_t size_audio = 0;
	uintmax_t size_image = 0;
	uintmax_t size_text = 0;

	/*
	 * проверять переполнение
	 * интов с размерами
	 * названий
	 */

	auto isThatType = [](auto types_vec, auto type)
	{
		auto iterator = std::find(types_vec.begin(), types_vec.end(), type);

		return iterator != types_vec.end();
	};


	for (auto & p : fs::recursive_directory_iterator(fs::current_path()))
	{
		std::wstring w_name = p.path();

		auto point = w_name.find_last_of(L".") + 1;
		std::wstring type = w_name.substr(point);

		uintmax_t size = getFileSizeByName(w_name);

		if (isThatType(types_image, type))
			size_image += size;

		if (isThatType(types_audio, type))
			size_audio += size; // ::file_size(p);

		if (isThatType(types_video, type))
			size_video += size;

		if (isThatType(types_text, type))
			size_text += size;
	}

	std::cout << "Images:\t" << static_cast<long double>(size_image) / KB / KB << " MB " << size_image << "\n";
	std::cout << "Video: \t" << static_cast<long double>(size_video) / KB / KB << " MB " << size_video << "\n";
	std::cout << "Audio: \t" << static_cast<long double>(size_audio) / KB / KB << " MB " << size_audio << "\n";
	std::cout << "Text:  \t" << static_cast<long double>(size_text) / KB / KB << " MB " << size_text << "\n";

	return 0;
}

uintmax_t getFileSizeByName(std::wstring name)
{
	if (name.size() < MAX_PATH)
		return fs::file_size(name);

	DWORD size = 0;

	/*
	 * https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
	 * In the ANSI version of this function, the name is limited to MAX_PATH characters.
	 * To extend this limit to 32,767 wide characters, call the Unicode version of the
	 * function and prepend "\\?\" to the path.
	 */
	HANDLE hFile = CreateFileW((L"\\\\?\\" + name).c_str(), GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return static_cast<uintmax_t>(size);
	}

	size = GetFileSize(hFile, nullptr);

	CloseHandle(hFile);
	return static_cast<uintmax_t>(size);

}
