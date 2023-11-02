#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include "SnippetsManager.h"
#include "MainWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSystemSemaphore semaphore("snippets_holder", 1);  // create semaphore
    semaphore.acquire(); // Raise the semaphore, barring other instances to work with shared memory

#ifndef Q_OS_WIN32
    // in linux / unix shared memory is not freed when the application terminates abnormally,
    // so you need to get rid of the garbage
    QSharedMemory nix_fix_shared_memory("snippets_holder_shared");
    if(nix_fix_shared_memory.attach()){
        nix_fix_shared_memory.detach();
    }
#endif

    QSharedMemory sharedMemory("snippets_holder_shared");  // Create a copy of the shared memory
    bool is_running;            // variable to test the already running application
    if (sharedMemory.attach())
    {                           // We are trying to attach a copy of the shared memory
        // To an existing segment
        is_running = true;      // If successful, it determines that there is already a running instance
    }
    else
    {
        sharedMemory.create(1); // Otherwise allocate 1 byte of memory
        is_running = false;     // And determines that another instance is not running
    }
    semaphore.release();

    // If you already run one instance of the application, then we inform the user about it
    // and complete the current instance of the application
//    SnippetsManager::sSetFusionPalette(&a);

    if (is_running)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("The application is already running.\n"
                       "Allowed to run only one instance of the application.");
        msgBox.exec();
        return 1;
    }

    SnippetsManager::sLoadStorage();
    MainWidget w;
    w.show();
    return a.exec();
}
