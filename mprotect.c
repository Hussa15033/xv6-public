#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(int argc, char *argv[])
{
    printf(1, "testing mprotect..\n");

    int NUMBER_OF_PAGES = 2;
    int * START_ADDR = (int *) 0x1000;

    // Check mprotect returns -1 for non-aligned address
    printf(1, "checking page alignment: ");
    if (mprotect((int *) 0x1001, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    // Check mprotect returns -1 for an address below the base of process.
    printf(1, "checking valid address lower-bound check: ");
    if (mprotect((int *) 0xFFF, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    // Check mprotect returns -1 for an address above the process limit (where kernel resides)
    printf(1, "checking valid address upper-bound check: ");
    if (mprotect((int *) KERNBASE, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    printf(1, "setting %d pages from address: %p to read-only\n", NUMBER_OF_PAGES, START_ADDR);

    if (mprotect(START_ADDR, NUMBER_OF_PAGES) != 0) {
        printf(1, "failed running mprotect.");
        exit();
    } else {
        printf(1, "successfully ran mprotect\n");
    }

    // 0x3000 is was not protected
    printf(1, "attempting to write to a page that is NOT read-only (should not cause a trap): ");
    int * mem = (int *) 0x3000;
    *mem = 5;
    printf(1, "success\n");
    printf(1, "attempting to write to page that is READ-ONLY (should cause a trap)\n");

    // 0x1000 should be protected
    int * rdonly = (int *) 0x1000;
    *rdonly = 5;

    // If a trap was not caused, mprotect has failed.
    printf(1, "\nFAILED, mprotect did not work.");
    exit();
}