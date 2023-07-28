#include "FileMgr.hpp"

#include "File.hpp"
#include "Folder.hpp"

namespace NewBase
{
	void FileMgr::Init(const std::filesystem::path& rootFolder)
	{
		GetInstance().InitImpl(rootFolder);
	}

	void FileMgr::InitImpl(const std::filesystem::path& rootFolder)
	{
		m_RootFolder = rootFolder;

		CreateFolderIfNotExists(m_RootFolder);
	}

	const std::filesystem::path& FileMgr::CreateFolderIfNotExists(const std::filesystem::path& folder)
	{
		bool create_path = !std::filesystem::exists(folder);

		if (!create_path && !std::filesystem::is_directory(folder))
		{
			std::filesystem::remove(folder);
			create_path = true;
		}

		if (create_path)
			std::filesystem::create_directory(folder);

		return folder;
	}

	const std::filesystem::path& FileMgr::EnsureFileCanBeCreated(const std::filesystem::path& file)
	{
		return FileMgr::CreateFolderIfNotExists(file.parent_path());
	}

	File FileMgr::GetProjectFileImpl(const std::filesystem::path& file) const
	{
		if (file.is_absolute())
			throw std::invalid_argument("FileMgr::GetProjectFile expects a relative path.");

		auto projFile = File(m_RootFolder / file);
		EnsureFileCanBeCreated(file);

		return projFile;
	}

	Folder FileMgr::GetProjectFolderImpl(const std::filesystem::path& folder) const
	{
		if (folder.is_absolute())
			throw std::invalid_argument("FileMgr::GetProjectFolder expects a relative path.");

		return {m_RootFolder / folder};
	}
}