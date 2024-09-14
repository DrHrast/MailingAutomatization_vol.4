#include "FolderWatcher.h"
#include "pch.h"

FolderWatcher::FolderWatcher(const std::wstring& directory)
    : m_directory(directory), m_hDir(INVALID_HANDLE_VALUE), m_stopWatching(false)
{
    m_hDir = CreateFileW(
        directory.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // Use overlapped I/O for async operation
        NULL
    );

    if (m_hDir == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to open directory handle." << std::endl;
    }
}

FolderWatcher::~FolderWatcher()
{
    if (m_hDir != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hDir);
    }
}

void FolderWatcher::StartWatching()
{
    if (m_hDir == INVALID_HANDLE_VALUE)
    {
        return;
    }

    OVERLAPPED overlapped = { 0 };
    overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    BYTE buffer[1024];
    DWORD bytesReturned;

    while (!m_stopWatching)  // Check if we should stop watching
    {
        if (ReadDirectoryChangesW(
            m_hDir,
            &buffer,
            sizeof(buffer),
            FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME, // Watch for file name changes (like new files)
            &bytesReturned,
            &overlapped,
            &FolderWatcher::CompletionRoutine))
        {
            WaitForSingleObject(overlapped.hEvent, 1000);
            // Continue watching unless stopWatching is set to true
        }
    }

    CloseHandle(overlapped.hEvent);
}

void FolderWatcher::StopWatching()
{
    m_stopWatching = true;
}

void CALLBACK FolderWatcher::CompletionRoutine(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped)
{
    if (dwErrorCode != ERROR_SUCCESS)
    {
        std::cerr << "Error watching directory changes: " << dwErrorCode << std::endl;
        return;
    }

    std::cout << "File change detected!" << std::endl;
}