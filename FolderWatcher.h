#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>

class FolderWatcher {
public:
    FolderWatcher(const std::wstring& directory); // Constructor
    ~FolderWatcher(); // Destructor

    void StartWatching(); // Starts the watching process
    void StopWatching(); // Stops the watching process

private:
    static void CALLBACK CompletionRoutine(
        DWORD dwErrorCode,
        DWORD dwNumberOfBytesTransfered,
        LPOVERLAPPED lpOverlapped);

    std::wstring m_directory; // Directory to watch
    HANDLE m_hDir; // Handle to the directory
    bool m_stopWatching; // Flag to stop watching
};