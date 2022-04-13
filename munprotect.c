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
    printf(1, "testing munprotect..\n");

    int NUMBER_OF_PAGES_TO_PROTECT = 2;
    int * START_ADDR = (int *) 0x1000;

    // Check munprotect returns -1 for non-aligned address
    printf(1, "checking page alignment: ");
    if (munprotect((int *) 0x1001, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    // Check munprotect returns -1 for an address below the base of process.
    printf(1, "checking valid address lower-bound check: ");
    if (munprotect((int *) 0xFFF, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    // Check munprotect returns -1 for an address above the process limit (where kernel resides)
    printf(1, "checking valid address upper-bound check: ");
    if (munprotect((int *) KERNBASE, 1) != -1) {
        printf(1, "failed\n");
        exit();
    } else {
        printf(1, "success\n");
    }

    printf(1, "using mprotect to protect %d pages from address: %p\n", NUMBER_OF_PAGES_TO_PROTECT, START_ADDR);
    if (mprotect(START_ADDR, NUMBER_OF_PAGES_TO_PROTECT) != 0) {
        printf(1, "failed running mprotect to temporarily protect pages.");
        exit();
    } else {
        printf(1, "successfully ran mprotect\n");
    }

    printf(1, "using munprotect to unprotect 1 page from address: %p\n", START_ADDR);
    if (munprotect(START_ADDR, 1) != 0) {
        printf(1, "failed running munprotect.");
        exit();
    } else {
        printf(1, "successfully ran munprotect\n");
    }

    // 0x1000 should not be protected
    int * mem = (int *) 0x1000;
    printf(1, "attempting to write to a page that is NOT read-only (0x%p) (should not cause a trap): ", mem);
    *mem = 5;
    printf(1, "success\n");


    // 0x2000 should not be protected
    int * rdonly = (int *) 0x2000;
    printf(1, "attempting to write to page that is READ-ONLY (0x%p) (should cause a trap)\n", rdonly);
    *rdonly = 5;

    // If a trap was not caused, munprotect has failed.
    printf(1, "\nFAILED, munprotect did not work.");
    exit();
}